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
  const connectionString =
    process.env.DATABASE_URL ||
    "postgres://mchonofsky:mchonofsky@localhost:5432/amtrak-lol";
  const client = new pg.Client({
    connectionString: connectionString,
  });
  client.connect();
  return client;
};

exports.saveStation = function saveStation(
  station,
  train_id,
  update_id,
  client
) {
  client
    .query(
      `
  INSERT INTO stations_trains (station_id, train_id, scheduled_arrival, estimated_arrival, posted_arrival, scheduled_departure, estimated_departure, posted_departure) 
  VALUES ( $1, $2, $3, $4, $5, $6, $7, $8 )
  ON CONFLICT (station_id, train_id) DO UPDATE SET
  scheduled_arrival = $3,
  estimated_arrival = $4,
  posted_arrival = $5,
  scheduled_departure = $6,
  estimated_departure = $7,
  posted_departure = $8;
              `,
      [
        station.code,
        train_id,
        station.schArr,
        station.estArr,
        station.postArr,
        station.schDep,
        station.estDep,
        station.postDep,
      ]
    )
    .catch((err) => {
      console.error("error saving station:", station.code,[                                                                                           
        station.code,                                                                                
        train_id,                                                                                    
        station.schArr,                                                                              
        station.estArr,                                                                              
        station.postArr,                                                                             
        station.schDep,                                                                              
        station.estDep,                                                                              
        station.postDep,] );
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
    INSERT INTO train_reports (name, amtrak_train_number, amtrak_object_id, latitude, longitude, velocity, heading, train_state, train_timely, service_disruption, original_scheduled_departure, updated_at, update_id)
    VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13)
    ON CONFLICT (original_scheduled_departure, amtrak_object_id)
    DO UPDATE SET 
    name=$1,
    amtrak_train_number=$2,
    amtrak_object_id=$3,
    latitude=$4, 
    longitude=$5, 
    velocity=$6, 
    heading=$7, 
    train_state=$8, 
    train_timely=$9, 
    service_disruption=$10, 
    updated_at=$12,
    update_id=$13
    RETURNING train_id;`,
      [
        train.routeName,
        train.trainNum,
        train.objectID,
        train.latitude,
        train.longitude,
        train.velocity,
        train.heading,
        train.trainState,
        train.trainTimely,
        train.serviceDisruption,
        train.origSchDep,
        train.updatedAt,
        update_id,
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
