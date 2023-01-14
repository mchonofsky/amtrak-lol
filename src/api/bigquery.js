const {BigQuery} = require('@google-cloud/bigquery');
const { createClient } = require('../db/setup.js');
const bigquery = new BigQuery();
const client = createClient();

exports.updateBigqueryStations = function updateBigqueryStations(client) {
  const query = `CREATE OR REPLACE TABLE \`amtracker.${process.env.BQ_DATASET}.stations\` AS (select * from \`amtracker.${process.env.BQ_DATASET}.stations\` limit 0);`;
  const options = {query: query, location: 'us-east4'};
  return Promise.all(
    [
      bigquery.dataset(process.env.BQ_DATASET)
        .table('stations')
        .createQueryJob(options),
      client.query('SELECT * FROM stations;')                                                          
    ]
  )                                                                                                 
  .then( (resultArray) => {                                                                          
    let queryResult = resultArray[1];                                                                
    try {                                                                                            
      bigquery                                                                                       
        .dataset(process.env.BQ_DATASET)                                                             
        .table('stations') // tables in db                                                           
        .insert(queryResult.rows);                                                                   
      console.log(`Inserted ${queryResult.rows.length} rows in stations`);                           
    } catch ( err ) {                                                                                
      console.log(err)                                                                               
      console.log(err.errors[0]);                                                                    
      return;                                                                                        
    }                                                                                                
  });                                                                                                
};                     

exports.insertBigqueryStations = function insertBigqueryStations( ) {
    // Insert data into a table
  const query = 'DELETE FROM stations;'
  const options = {query: query, location:'us-east4'}
  
  return Promise.all([
    bigquery
      .dataset(process.env.BQ_DATASET)
      .table('stations') // tables in db
      .createQueryJob(options), 
    client.query('SELECT * FROM stations;')
  ])
  .then( (resultArray) => {
    let queryResult = resultArray[1];
    try {
      console.log('QUERY RESULT', queryResult.rows);
      bigquery
        .dataset(process.env.BQ_DATASET)
        .table('stations') // tables in db
        .insert(queryResult.rows);
      console.log(`Inserted ${queryResult.rows.length} rows in stations`);
    } catch ( err ) {
      console.log(err)
      console.log(err.errors[0]);
      return;
    }
  });
};

exports.insertBigqueryStationsTrains = async function insertBigqueryStationsTrains(max_update_id ) {
  const queryResult = await client.query(
    `SELECT * FROM stations_trains WHERE update_id != ${max_update_id}`) // stops updating when train disappears
  const rows = queryResult.rows;  
  if (rows.length == 0) return;
  
  // Insert data into a table
  await bigquery
    .dataset(process.env.BQ_DATASET)
    .table('stations_trains') // tables in db
    .insert(queryResult.rows);
  console.log(`Inserted ${rows.length} rows in BQ to ${process.env.BQ_DATASET}.stations_trains`);
};

exports.insertBigqueryTrainReports = async function insertBigqueryTrainReports(max_update_id ) {
  const queryResult = await client.query(
    `SELECT * FROM train_reports;`)
  const rows = queryResult.rows;  
  if (rows.length == 0) return;
  
  // Insert data into a table
  await bigquery
    .dataset(process.env.BQ_DATASET)
    .table('train_reports') // tables in db
    .insert(queryResult.rows);
  console.log(`Inserted ${rows.length} rows in BQ to ${process.env.BQ_DATASET}.train_reports`);
};
