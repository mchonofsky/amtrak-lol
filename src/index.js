const express = require('express');
const crypto = require('crypto');
const path = require('path');
const pg = require('pg');
const amtrak = require('amtrak');
const PORT = process.env.PORT || 3000;
const DATA_SALT = process.env.DATA_SALT || ""

// Create a new express app
const app = express();
app.use(express.json());

app.post("/echo", (req, res) => {
  res.json(req.body);
}); 

app.post('/api/load_data', (req, res) =>
  {
    let data = req.body;
    if (DATA_SALT == data.api_key) {
      console.log('challenge succeeded');
      res.status(200);
      res.send('success\n');
    } else {
      console.log('challenge failed\n got', data.api_key);
      res.status(403);
      res.send('failed\n');
    }
  }
);

app
  .use(express.static(path.join(__dirname, 'public')))
  .set('views', path.join(__dirname, 'views'))
  .set('view engine', 'ejs')
  .get('/', (req, res) => res.render('pages/index'))
  .listen(PORT, () => console.log(`Listening on ${ PORT }`))

// Start the app on the specified port
/*
 const port = process.env.PORT || 3000;
app.listen(port, () => {
  console.log(`App listening on port ${port}`);
});
*/

