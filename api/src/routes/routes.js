const express =  require('express');
const router = express.Router();
const controller = require('../controllers/controller');

// ***************************************************************************
// TEMPERATURA
// ***************************************************************************
router.get('/getTemperatura', controller.obtenerTemperatura);
router.post('/addTemperatura', controller.insertarTemperatura);

// ***************************************************************************
// DISTANCIA
// ***************************************************************************
router.get('/getDistancia', controller.obtenerDistancia);
router.post('/addDistancia', controller.insertarDistancia);

// ***************************************************************************
// ILUMINACION
// ***************************************************************************
router.get('/getIluminacion', controller.obtenerIluminacion);
router.post('/addIluminacion', controller.insertarIluminacion);

// ***************************************************************************
// AIRE
// ***************************************************************************
router.get('/getAire', controller.obtenerAire);
router.post('/addAire', controller.insertarAire);

// ***************************************************************************
// HUMEDAD
// ***************************************************************************
router.get('/getHumedad', controller.obtenerHumedad);
router.post('/addHumedad', controller.insertarHumedad);

module.exports = router;