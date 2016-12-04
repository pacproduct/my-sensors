/**
 * Handles serial communications with a tiers component.
 */

var util = require("util");
var EventEmitter = require("events").EventEmitter;
var SerialPortLib = require('serialport');

/**
 * Default options when creating a new instance of this library.
 * See constructor for details.
 */
var defaultOptions = {
  pathToSerialPort: '/dev/ttyACM0',
  protocolBaudRate: 9600,
  protocolMsgEndChar: '\n',
  protocolMsgMaxLength: 512,
  autoReconnect: true,
  autoReconnectEveryMs: 500,
};

/**
 * Constructor.
 *
 * @param object options
 *   Options this instance should be initialized with.
 *   {
 *     // Linux path to the serial port the tiers component is connected to.
 *     pathToSerialPort: (string),
 *     // Character used to detect the end of a serial message.
 *     protocolMsgEndChar: (1 char long string),
 *     // Maximum number of bytes a single message can hold. If trying to
 *     // send a message longer than that
 *     protocolMsgMaxLength: (int),
 *     // Serial port speed (in bauds). Should be one of 115200, 57600, 38400,
 *     // 19200,9600, 4800, 2400, 1800, 1200, 600, 300, 200, 150, 134, 110, 75
 *     // or 50.
 *     protocolBaudRate: (int),
 *     // Set to TRUE if you want the serial link to try reconnecting
 *     // automatically if it drops unexpectedly. If set to FALSE, the
 *     // connection will stay closed when it drops.
 *     autoReconnect: (bool),
 *     // When the autoReconnect option is enabled, it should wait the following
 *     // amount of milliseconds before trying to reconnect again.
 *     autoReconnectEveryMs: (int),
 *   }
 */
function SerialCom(options) {
  if (typeof options !== 'object') {
    options = {};
  }

  // Parse options.
  this.options = parseOptions(defaultOptions, options);

  // SerialPort object used for serial communication.
  this.serialPortConnection = null;
  // This turns to TRUE when the user asks this connection to close itself.
  // Is used to know when the autoReconnect option should not be taken into
  // account.
  this.connectionBeingClosedByUser = false;
}

util.inherits(SerialCom, EventEmitter);

/**
 * Opens a serial connection to tiers component.
 *
 * @param function callback
 *   Function called when the open operation completed. Signature:
 *   function(err) {}
 *   - Error err: Error object if something went wrong.
 *     Is null/undefined otherwise.
 */
SerialCom.prototype.openConnection = function(callback) {
  var that = this;
  var conOpen = false;

  that.connectionBeingClosedByUser = false;
  that.serialPortConnection = new SerialPortLib(
    that.options.pathToSerialPort,
    {
      baudRate: that.options.protocolBaudRate,
      parser: SerialPortLib.parsers.readline(that.options.protocolMsgEndChar),
    }
  );

  // Subscribe to 'error' event.
  that.serialPortConnection.on('error', function(err) {
    if (!conOpen) {
      return callback(err);
    }
    else {
      return that.emit('error', err);
    }
  });

  //Subscribe to 'open' event.
  that.serialPortConnection.on('open', function() {
    // Flush any data that could be lingering in a buffer.
    that.serialPortConnection.flush(function(err) {
      if (err) {
        return callback(err);
      }

      // Connection now opened and ready.
      // Reference what to do with events.
      // -> When data is available on the serial port.
      conOpen = true;
      that.serialPortConnection.on('data', function(data) {
        return that.emit('data', data);
      });
      // -> When the connection drops.
      that.serialPortConnection.on('close', function(err) {
        that.emit('close');

        // If auto reconnection is disabled, simply emit the close event.
        // If user asked for closing the connection, same thing.
        if (!that.options.autoReconnect || that.connectionBeingClosedByUser) {
          return;
        }
        // If connection is closing and if auto-reconnection is enabled,
        // try reconnecting.
        else {
          that.emit('reconnecting');
          that._runAutoReconnectSequence();
        }
      });

      return callback(null);
    });
  });
};

/**
 * Internal function run when the autoReconnect option is enable.
 * Tries continuously to reconnect until the connection is up again.
 * Stops trying when:
 * - Connection is up again.
 * - User asks to close the connection.
 */
SerialCom.prototype._runAutoReconnectSequence = function() {
  var that = this;

  if (typeof that.serialPortConnection !== 'undefined' && that.serialPortConnection.isOpen()) {
    // Connection is up again, yay, we're done here.
    return;
  }

  // If user asked to close the connection, stop trying.
  if (that.connectionBeingClosedByUser) {
    return;
  }

  that.openConnection(function(err) {
    // Try again in a bit of time.
    // If connection succeeded, the next call will exit promptly.
    setTimeout(
      function() {
        that._runAutoReconnectSequence();
      },
      that.options.autoReconnectEveryMs
    );
  });
};

/**
 * Opens a serial connection to tiers component.
 *
 * @param boolean autoReconnect
 *   Set to TRUE if you want the serial link to try reconnecting automatically
 *   if it drops unexpectedly. If set to FALSE, any issue with the connection
 *   would simply close it
 * @param function callback
 *   Function called when the close operation completed. Signature:
 *   function(err) {}
 *   - Error err: Error object if something went wrong.
 *     Is null/undefined otherwise.
 */
SerialCom.prototype.closeConnection = function(callback) {
  this.connectionBeingClosedByUser = true;
  this.serialPortConnection.close(function(err){
    if (err) {
      return callback(err);
    }

    return callback(null);
  });
};

/**
 * Sends data to the tiers component.
 *
 * @param string message
 *   Message to send to the tiers component.
 *   Must be <= than options.protocolMsgMaxLength, otherwise it will be
 *   truncated without notice.
 *   When this happens, if input message contained characters encoded on more
 *   than 8 bits (e.g. UTF8), you migth end up with invalid characters!
 * @param function sendStartedCallback
 *   Function called when the send operation started. Signature:
 *   function(err) {}
 *   - Error err: Error object if something went wrong.
 *     Is null/undefined otherwise.
 * @param function sendCompleteCallback
 *   [Optional]
 *   Function called when the send operation completed. Signature:
 *   function(err) {}
 *   - Error err: Error object if something went wrong.
 *     Is null/undefined otherwise.
 */
SerialCom.prototype.send = function(message, sendStartedCallback, sendCompleteCallback) {
  var that = this;

  var messageBuffer = Buffer.from(message);

  if (messageBuffer.length > that.options.protocolMsgMaxLength) {
    message = messageBuffer.slice(0, that.options.protocolMsgMaxLength);
  }

  that.serialPortConnection.write(toSend + that.options.protocolMsgEndChar, function(err) {
    if (err) {
      return sendStartedCallback(err);
    }

    if (typeof sendCompleteCallback === 'function') {
      that.serialPortConnection.drain (function(err){
        if (err) {
          return sendCompleteCallback(err);
        }

        return sendCompleteCallback(null);
      });
    }

    return sendStartedCallback(null);
  });
};




/**
 * Utility function.
 * Merge single level option objects. Does not go deeper than 1st level.
 *
 * @param object defaultOptions.
 *   An object of default options.
 * @param object options
 *   An object of options.
 *
 * @return object
 *   All default options overriden by given options.
 */
function parseOptions(defaultOptions, options) {
  finalOptions = {};
  if (typeof options === 'object') {
    finalOptions = options;
  }

  for (var optName in defaultOptions) {
    if (defaultOptions.hasOwnProperty(optName) && !options.hasOwnProperty(optName)) {
      finalOptions[optName] = defaultOptions[optName];
    }
  }

  return finalOptions;
}




// Export this module.
module.exports = SerialCom;
