/**
 * Handles the communication between the base and satellites.
 */

var EventEmitter = require("events").EventEmitter;

/**
 * Constructor.
 *
 * @param object serialCom
 *   A configured serialCom instance for exchanging with satellites.
 */
function senseCom(serialCom) {
  this.serialCom = serialCom;


}

util.inherits(SerialCom, EventEmitter);




/**
 * Opens the link to satellite sensors, and listens to incoming messages.
 *
 * @param function callback
 *   Function called when the open operation completed. Signature:
 *   function(err) {}
 *   - Error err: Error object if something went wrong.
 *     Is null/undefined otherwise.
 */
senseCom.prototype.openConnection = function(callback) {
  // Connection.
  serialCom.openConnection(function(err) {
    if (err) {
      return callback(err);
    }

    callback(null);
  });
}




// Export this module.
module.exports = SerialCom;
