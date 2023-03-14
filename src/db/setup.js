const process = require("process");
const pg = require("pg");

exports.DEBUG_MODE = process.env.ENVIRONMENT == "local" ? true : false;
exports.rollback = function rollback(client) {
  // rollback the transaction in case of an error
  client.query("ROLLBACK", (err) => {
    if (err) {
      console.error("Could not rollback the transaction: ", err);
    }
  });
};

exports.createClient = function createClient() {
  const connectionString = process.env.HEROKU_POSTGRESQL_AQUA_URL 
    || process.env.DATABASE_URL || "postgres://mchonofsky:mchonofsky@localhost:5432/amtrak-lol";
  const client = new pg.Client({
    connectionString: connectionString,
    ssl: {
      rejectUnauthorized: false,
    },
  });
  client.connect();
  return client;
};

exports.sanitizeTime = function sanitizeTime(timeString) {
  //
  if (parseInt(timeString.slice(11,13)) > 23) {
    var date_part = timeString.slice(0,10);
    var date_as_date = new Date(date_part);
    // add a day
    date_as_date.setSeconds(date_as_date.getSeconds() + 86400);
    var date_as_string = date_as_date.toISOString().slice(0, 11);
    var hours_as_string = (parseInt(timeString.slice(11,13)) - 24).toString().padStart(2,'0')
  } else
  {
    var date_as_string = timeString.slice(0,11);
    var hours_as_string = timeString.slice(11,13);
  }
  
  var finalTimeString = date_as_string + hours_as_string + timeString.slice(13);
  
  try {
    return new Date(Date.parse(finalTimeString));
  }
  catch ( e ) {
    console.log('ERROR: timeString is', timeString);
    throw e;
  }
}

exports.bestTime = function(station_record) {
  // falls down the list to enable accurate sorting of rows
  return [
    station_record.scheduled_arrival,
    station_record.scheduled_departure,
    station_record.posted_arrival,
    station_record.posted_departure
  ].reduce( (time, option) => (time === undefined) && (! ( option === undefined)) ? option : time )
}
