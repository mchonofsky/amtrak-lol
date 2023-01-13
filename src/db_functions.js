const process = require("process");
const pg = require("pg");
const amtraker = require("./amtraker_api.js");

exports.rollback = function rollback(client) {
  // rollback the transaction in case of an error
  client.query("ROLLBACK", (err) => {
    if (err) {
      console.error("Could not rollback the transaction: ", err);
    }
  });
};

exports.createClient = function createClient() {
  console.log("process.env.DB_U", process.env.DATABASE_URL);
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

exports.upsertStation = function upsertStation(station_code) {
  return amtraker_api
    .fetchOneStation(station_code)
    .then((res) => {
      return client.query(
        `INSERT INTO stations (name, station_id, latitude, longitude, state_code)
    VALUES ($1, $2, $3, $4, $5) ON CONFLICT (station_id) DO UPDATE SET
        name = $1;`,
        [
          res[station_code].name,
          station_code,
          res[station_code].lat,
          res[station_code].lon,
          res[station_code].state,
        ]
      );
    })
    .catch(() => {
      return client.query(
        `INSERT INTO stations (station_id)
    VALUES ($1, $2);`,
        ["Unknown", station_code]
      );
    });
};

exports.saveStation = function saveStation(
  station,
  train_id,
  update_id,
  client
) {
  return client
    .query(
      `
  INSERT INTO stations_trains (station_id, train_id, scheduled_arrival, posted_arrival, scheduled_departure, posted_departure) 
  VALUES ( $1, $2, $3, $4, $5, $6 )
  ON CONFLICT (station_id, train_id) DO UPDATE SET
  scheduled_arrival = $3,
  posted_arrival = $4,
  scheduled_departure = $5,
  posted_departure = $6;
              `,
      [
        station.code,
        train_id,
        station.schArr,
        station.arr,
        station.schDep,
        station.dep,
      ]
    )
    .catch((err) => {
      console.error("error saving station:", station.code, [
        station.code,
        train_id,
        station.schArr,
        station.arr,
        station.schDep,
        station.dep,
      ]);
      if (
        !err.message ==
        "error: current transaction is aborted, commands ignored until end of transaction block"
      )
        console.log("error is", err);
      client.query("ROLLBACK", (err) => {
        if (err) {
          console.error("Could not rollback the transaction: ", err);
        }
      });
      throw err;
    });
};

exports.saveTrain = function saveTrain(train, update_id, client) {
  console.log("inserting train", train.routeName, train.objectID);
  return client
    .query(
      `
    INSERT INTO train_reports (name, amtrak_train_number, amtrak_object_id, latitude, longitude, velocity, heading, train_state, service_disruption, updated_at, update_id)
    VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)
    ON CONFLICT (amtrak_object_id)
    DO UPDATE SET 
    name=$1,
    amtrak_train_number=$2,
    amtrak_object_id=$3,
    latitude=$4, 
    longitude=$5, 
    velocity=$6, 
    heading=$7, 
    train_state=$8, 
    service_disruption=$9,
    updated_at=$10,
    update_id=$11
    RETURNING train_id;`,
      [
        train.routeName, //1
        train.trainNum, //2
        train.trainID,//3
        train.lat, //4
        train.lon, //5
        train.velocity, // 6
        train.heading, // 7
        train.trainState, // 8 
        train.serviceDisruption, // 9
        train.updatedAt, // 10
        update_id, // 11
      ]
    )
    .catch((err) => {
      console.error(
        "error saving train:",
        train.routeName,
        train.trainNum,
        train.velocity,
        train.heading
      );
      console.log(train);
      if (
        !err.message ==
        "error: current transaction is aborted, commands ignored until end of transaction block"
      )
        console.log("error is", err);
      client.query("ROLLBACK", (err) => {
        if (err) {
          console.error("Could not rollback the transaction: ", err);
        }
      });
      throw err;
    });
};
