const express = require('express');
const morgan = require("morgan");
const mysql = require('mysql')
const myConnection = require('express-myconnection');
const corse = require('cors')
const bodyparser = require('body-parser');
const routesAPI = require('./routes/routes')
const app = express();

// ***************************************************************************
// SETTINGS
// ***************************************************************************
app.set('port', process.env.PORT || 3010);

// ***************************************************************************
// MMIDDLEWARES
// ***************************************************************************
app.use(morgan('dev'));
app.use(express.json())
app.use(corse());

app.use(express.json({ limit: '50mb' }));
app.use(bodyparser.json({ limit: '50mb' }));
app.use(bodyparser.urlencoded({ limit: '50mb' }));


app.use(myConnection(mysql, {
    host: 'database-1.cf62bgtvk4l3.us-east-2.rds.amazonaws.com',
    user: 'admin',
    password: 'adminadmin',
    port: '3306',
    database: 'fase3'
}, 'single'))

// ***************************************************************************
// RUTAS
// ***************************************************************************

app.use('/', routesAPI)

// ***************************************************************************
// MANEJO DE ERRORES
// ***************************************************************************
app.use((err, req, res, next) => {
    console.error('Error no capturado:', err);
    res.status(500).json({ error: 'Ha ocurrido un error en el servidor.' });
});

app.listen(app.get('port'), () => {
    console.log('Server on port 3010');
});
