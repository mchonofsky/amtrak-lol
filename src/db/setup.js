const process = require("process");
const pg = require("pg");

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