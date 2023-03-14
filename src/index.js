require('dotenv').config();
const fs = require('fs');
const express = require("express");
const crypto = require("crypto");
const path = require("path");
const pg = require("pg");
const amtraker = require("./api/amtraker_api.js");

const PORT = process.env.PORT || 3000;
const DATA_SALT = process.env.DATA_SALT || "";

const { DEBUG_MODE, createClient, bestTime } = require("./db/setup.js")
const { archive, upsertStation, saveTrain, saveStation, purge} = require("./db/update.js");

const client = createClient();

// Create a new express app
const app = express();

app.use(express.json());

app.post("/api/get_train", async (req, result) => {
  let data = req.body;
  console.log('geoloc data is.', data)
  // get dummy data
  let trains = (await client.query(
        `SELECT st.train_id train_id, s1.station_id first_station_station_id, s1.city first_station_name, s1.state_code first_station_state_code, s2.station_id last_station_station_id, s2.city last_station_name, s2.state_code last_station_state_code, 
        JSON_AGG(
        json_build_object(
        'station_id',st.station_id, 
        'scheduled_arrival', st.scheduled_arrival, 
        'scheduled_departure',st.scheduled_departure, 
        'posted_arrival', st.posted_arrival, 
        'posted_departure', st.posted_departure,
        'display_name', sx.station_name
      )) details
          FROM stations_trains st JOIN train_reports tr ON st.train_id = tr.train_id
          JOIN stations s1 ON s1.station_id = tr.first_station
          JOIN stations s2 on s2.station_id = tr.last_station
          JOIN stations sx on st.station_id = sx.station_id
        WHERE ABS(-122.2-tr.longitude) < 0.5
        GROUP BY st.train_id, s1.city, s1.state_code, s2.city, s2.state_code, s1.station_id, s2.station_id;`)).rows;
  result.status(200);
  result.send(trains);
})

app.post("/api/train_details", async (req, result) => {
  let data = req.body.train_id;
  if (! Number.isInteger(data)) {
    result.status(403);
    result.send();
  }

  var stations_trains = (await
    client.query(`SELECT
      train.amtrak_train_number, train.velocity, train.lastvalts, train.first_station, train.last_station,
      stations_trains.*, stations.display_name, stations.state_code, stations.latitude, stations.longitude
      FROM train_reports train
      JOIN stations_trains ON train.train_id = stations_trains.train_id
      JOIN stations ON stations.station_id = stations_trains.station_id
      WHERE train.train_id = $1
      ORDER BY stations_trains.scheduled_arrival`, [data])).rows;
  console.log(stations_trains);
  console.log('STATIONS_TRAINS');
  const resultToOutput = (st) => {
    return {
        station_id: st.station_id,
        scheduled_arrival: st.scheduled_arrival,
        scheduled_departure: st.scheduled_departure,
        posted_arrival: st.posted_arrival,
        posted_departure: st.posted_departure,
        display_name: st.display_name,
        state_code: st.state_code,
        latitude: st.latitude,
        longitude: st.longitude
    }
  };

  let first_station_station_code = stations_trains[0].first_station;
  let last_station_station_code  = stations_trains[0].last_station;
  first_station = resultToOutput(stations_trains.reduce((x, opt) => opt.station_id == first_station_station_code ? opt : x));
  last_station  = resultToOutput(stations_trains.reduce((x, opt) => opt.station_id == last_station_station_code  ? opt : x));

  //TODO remove time
  let train_data = {
    current_time: new Date((first_station.scheduled_arrival.getTime() + last_station.scheduled_arrival.getTime())/2), // DEBUGGG!!!!
    train_data: {
      amtrak_train_number: stations_trains[0].amtrak_train_number,
      velocity: stations_trains[0].velocity,
      lastvalts: stations_trains[0].lastvalts
    },
    stations: stations_trains.map(resultToOutput ),
    first_station: first_station,
    last_station : last_station
  };

  result.status(200);
  result.send(train_data);
  return;
})

app.post("/api/get_stations", async (req, result)  => {
  stations = (await client.query('SELECT display_name, city, name, state_code, station_id FROM stations;')).rows;
  result.status(200);
  result.send(stations);
});

app.post("/api/search_trains", async (req, result) => {
  // gets N UNTRUSTED station_codes
  try {
    let station_code_regex = /[A-Z][A-Z][A-Z]/
    let data = req.body.station_codes.filter( station_code => station_code.match(station_code_regex));
    console.log('after data:', data);
    var unique_station_ids = [...new Set(data)];
  } catch {
    console.log('could not find station codes');
    console.log('request body is', req.body)
  }
  console.log('unique station ids for search', unique_station_ids);
  // TODO use public_train_id that doesn't directly ref database index
  if (unique_station_ids.length > 0 ) {
    station_id_string = "'" + unique_station_ids.join("','") + "'"
    var matching_trains = (
      await client.query(
        `SELECT st.train_id train_id, s1.station_id first_station_station_id, s1.city first_station_name, s1.state_code first_station_state_code, s2.station_id last_station_station_id, s2.city last_station_name, s2.state_code last_station_state_code, 
        JSON_AGG(
        json_build_object(
        'station_id',st.station_id, 
        'scheduled_arrival', st.scheduled_arrival, 
        'scheduled_departure',st.scheduled_departure, 
        'posted_arrival', st.posted_arrival, 
        'posted_departure', st.posted_departure,
        'display_name', sx.station_name
      )) details
          FROM stations_trains st JOIN train_reports tr ON st.train_id = tr.train_id
          JOIN stations s1 ON s1.station_id = tr.first_station
          JOIN stations s2 on s2.station_id = tr.last_station
          JOIN stations sx on st.station_id = sx.station_id
        WHERE st.station_id IN (${station_id_string}) 
        GROUP BY st.train_id, s1.city, s1.state_code, s2.city, s2.state_code, s1.station_id, s2.station_id 
      HAVING COUNT(DISTINCT st.station_id) = ${unique_station_ids.length};`)
      ).rows;
  } else {
    var matching_trains = [];
  }
  console.log('sending', matching_trains)
  result.status(200);
  result.send(matching_trains);
});

app.post("/api/purge", (req, result) => {
  let data = req.body;
  if (DATA_SALT == data.api_key) {
    purge(client);
  }
});

app.post("/api/load_data", (req, result) => {
  let data = req.body;
  let update_id = undefined;
  let all_stations = [];
  let total_length = 0;
  // TODO rewrite with async/await
  if (DATA_SALT == data.api_key) {
    console.log("challenge succeeded");
    client.query("INSERT INTO db_state VALUES (DEFAULT, DEFAULT) RETURNING update_id;")
    .then(
      (res) => {
        update_id = res.rows[0].update_id;
        return client.query("SELECT DISTINCT station_id FROM stations;")
      },
      (err) => {
        console.log("error getting update_id", (update_id, null));
        console.log(err);
      }
    )
    .then(
      (res) => {
        all_stations = res.rows.map((item) => item.station_id);
      },
      (err) => {
        console.log('error getting stations');
        console.log(err);
      }
    )
    .then( (res) => {
      //let total_length = 0;
      return client.query("BEGIN");
    })
    .then(
      amtraker.fetchAllTrains,
      (err) => {
        console.log("error fetching train data");
        console.log(err);
      }
    )
    .then((data) => {
      // return Promise when all trains are uploaded
      console.log('Updating. update_id is ', update_id);
      if ( DEBUG_MODE ) console.log('keys:', Object.keys(data));
      const train_entries = Object.values(data);
      return Promise.all([].concat(...train_entries).map((train) => {
        if ( DEBUG_MODE ) console.log("train data", train.routeName, train.routeNumber, train.objectID);
        // save the train
        saveTrain(train, update_id, client)
        // then save the stations the train is going through
        .then(
          (train_id) => {
            total_length += 1;
            console.log("stations:", train.stations.length);
            return Promise.all(
              train.stations.map((station) => {
                if ( DEBUG_MODE ) console.log(total_length, "committing station", station.code);
                total_length += 1;
                if ( all_stations.includes ( station.code ) ) {
                  return saveStation(station, train_id.rows[0].train_id, update_id, client);
                } else {
                  return upsertStation(station.code, client).then(() =>
                    {
                      saveStation(station, train_id.rows[0].train_id, update_id, client);
                    }
                  );
                }
            // the promise for all stations
              })
            );
          }
        );
      // this is the promise for all trains
      }));
    },
    (err) => {
      console.log('error getting train data');
      console.log(err);
    })
    .then((res) => {
      return client.query("COMMIT");
    })
    .then((res) => {
      console.log("Transaction committed.");
    })
    .then((res) => {
      archive(client);
    })
    .then((res) => {
      purge(client);
    })
    .then((res) => {
      result.status(200);
      result.send({result: "ok"});
      return;
    })
    .catch(err => console.log('overall update error', err));
  } else {
    console.log("challenge failed\n got", data.api_key);
    result.status(403);
    result.send("failed\n");
  }
});

app
  .use(express.static(path.join(__dirname, "../amtrak-lol", "build")))
  //.set("views", path.join(__dirname, "views"))
  //.set("view engine", "ejs")
  .get("/*", (req, res) => res.sendFile(path.join(__dirname,"../amtrak-lol/build/index.html")))
  .listen(PORT, () => console.log(`Listening on ${PORT}`));

