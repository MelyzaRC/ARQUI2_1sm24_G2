/*
 Define las funciones que interactuan directamente
 con la base de datos
 */
 const mariadb = require('mariadb');
 require('dotenv').config({ path: './credenciales/db.env' });
 
 // Configura la conexión a la base de datos
 const pool = mariadb.createPool({
   host: process.env.DB_HOST,
   user: process.env.DB_USER,
   password: process.env.DB_PASSWORD,
   database: process.env.DB_NAME
 });
 
 // Función que ejecuta un query sin parametros a la base de datos
 async function ejecutar_query_simple(query) {
   let conexion;
   try {
     conexion = await pool.getConnection();
     const rows = await conexion.query(query);
     return rows;
   } catch (err) {
     console.error(err);
     throw err;
   } finally {
     if (conexion) {
       conexion.release();
     }
   }
 }
 
 // Función que ejecuta un query con parametros a la base de datos
 async function ejecutar_query(query, values) {
   let conexion;
   try {
     conexion = await pool.getConnection();
     const rows = await conexion.query(query, values);
     return rows;
   } catch (err) {
     console.error(err);
     throw err;
   } finally {
     if (conexion) {
       conexion.release();
     }
   }
 }
 
 module.exports = { ejecutar_query_simple, ejecutar_query };
 