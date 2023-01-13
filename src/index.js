const express = require("express");
const crypto = require("crypto");
const path = require("path");
const pg = require("pg");
const amtrak = require("./amtrak");
const amtraker = require("./amtraker_api.js");

const PORT = process.env.PORT || 3000;
const DATA_SALT = process.env.DATA_SALT || "";

const { insertStation, saveTrain, saveStation, createClient } = require("./db_functions.js");

const client = createClient();

// Create a new express app
const app = express();
app.use(express.json());

app.post("/api/load_data", (req, result) => {
  let data = req.body;
  let update_id = undefined;
  let all_stations = [];
  let total_length = 0;
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
      console.log('update_id is ', update_id);
      console.log('keys:', Object.keys(data));
      const train_entries = Object.values(data);
      return Promise.all([].concat(...train_entries).map((train) => {
        console.log("train data", train.routeName, train.routeNumber, train.objectID);
        // save the train
        saveTrain(train, update_id, client)
        // then save the stations the train is going through
        .then(
          (train_id) => {
            total_length += 1;
            console.log("stations:", train.stations.length);
            return Promise.all(
              train.stations.map((station) => {
                console.log(total_length, "committing station", station.code);
                total_length += 1;
                if ( all_stations.includes ( station.code ) ) {
                  return saveStation(station, train_id.rows[0].train_id, update_id, client);
                } else { 
                  return insertStation(station.code).then(() =>
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
  .use(express.static(path.join(__dirname, "public")))
  .set("views", path.join(__dirname, "views"))
  .set("view engine", "ejs")
  .get("/", (req, res) => res.render("pages/index"))
  .listen(PORT, () => console.log(`Listening on ${PORT}`));

// Start the app on the specified port
/*
 const port = process.env.PORT || 3000;
app.listen(port, () => {
  console.log(`App listening on port ${port}`);
});
*/
