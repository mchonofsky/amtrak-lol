const process = require("process");
const pg = require("pg");
const amtraker = require("../api/amtraker_api.js");
const {rollback, createClient } = require('./setup.js');
const { insertBigqueryTrainReports, insertBigqueryStationsTrains } = require('../api/bigquery.js')

client = createClient();

exports.upsertStation = function upsertStation(station_code, client) {
  return amtraker
    .fetchOneStation(station_code)
    .then((res) => {
      return client.query(
        `INSERT INTO stations (name, station_id, latitude, longitude, state_code)
    VALUES ($1, $2, $3, $4, $5) ON CONFLICT (station_id) DO UPDATE SET
        name = $1,
        station_id = $2,
        latitude = $3,
        longitude = $4,
        state_code = $5;`,
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
        `INSERT INTO stations (name, station_id)
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
  if (! ((station.schArr == null) && (station.schDep == null) )) { 
    return client
      .query(
        `
    INSERT INTO stations_trains (station_id, train_id, scheduled_arrival, posted_arrival, scheduled_departure, posted_departure, update_id) 
    VALUES ( $1, $2, $3, $4, $5, $6, $7 )
    ON CONFLICT (station_id, train_id) DO UPDATE SET
    scheduled_arrival = $3,
    posted_arrival = $4,
    scheduled_departure = $5,
    posted_departure = $6,
    update_id = $7;
                `,
        [
          station.code,
          train_id,
          station.schArr,
          station.arr,
          station.schDep,
          station.dep,
          update_id
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
        rollback(client);
        throw err;
      });
  } else {
    client.query('');
  }
};

exports.saveTrain = function saveTrain(train, update_id, client) {
  console.log("inserting train", train.routeName, train.objectID);
  return client
    .query(
      `
    INSERT INTO train_reports (name, amtrak_train_number, amtrak_object_id, latitude, longitude, velocity, heading, train_state, service_disruption, updated_at, update_id, lastValTS)
    VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)
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
    update_id=$11,
    lastValTS=$12
    RETURNING train_id;`,
      [
        train.routeName, //1
        train.trainNum, //2
        train.objectID, // was trainID,//3
        train.lat, //4
        train.lon, //5
        train.velocity, // 6
        train.heading, // 7
        train.trainState, // 8 
        train.serviceDisruption, // 9
        train.updatedAt, // 10
        update_id, // 11
        train.lastValTS // 12
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
      rollback(client);
      throw err;
    });
};

exports.updateBigqueryStations = function updateBigqueryStations(client) {

}

exports.archive = async function archive(client) {
  // ARCHIVE streams ALL train_reports to BigQuery
  // It also streams SELECTED stations_trains to BigQuery where old version had null and new version doesn't
  let update_id = (await client.query('SELECT MAX(update_id) FROM db_state;')).rows[0].max;
  return Promise.all (
    [insertBigqueryTrainReports(update_id ), insertBigqueryStationsTrains(update_id )]
  );
}

exports.purge = function purge(client) {
  return client.query('SELECT MAX(update_id) FROM db_state;')
  .then( (res) => {
      update_id = res.rows[0].max;  
  })
  .then( (res) => client.query('BEGIN;'))
  .then ((res) => client.query('DELETE FROM stations_trains WHERE update_id != $1 or update_id is null;', [update_id]) ) 
  .then ((res) => client.query('DELETE FROM train_reports WHERE update_id != $1 or update_id is null;', [update_id]) ) 
  .then ((res) => {
    client.query('COMMIT;');
    console.log('Purge complete.');
  });
}
