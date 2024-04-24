const mysql = require('mysql');
const controller = {}

// ***************************************************************************
// TEMPERATURA
// ***************************************************************************

// INSERT
controller.insertarTemperatura = async (req, res) => {
    const { temperatura } = req.body
    req.getConnection(async (err, conn) => {
        if (err) throw (err)
        const insertarSql = "INSERT INTO temperatura(temperatura,fecha) values (?, now())"
        const insertar_query = mysql.format(insertarSql, [temperatura])
        await conn.query(insertar_query, (err, result) => {
            if (err) {
                res.status(500).send("No se ha podido registrar la temperatura")
                return;
            }
            res.status(200).send("Temperatura registrada")
        })
    })
}

// SELECT
controller.obtenerTemperatura = async (req, res) => {
    req.getConnection((err, conn) => {
        conn.query('SELECT * FROM temperatura', (err, temperatura) => {
            if (err) {
                res.status(404).json(err)
                return;
            }
            res.status(200).json(temperatura);
        });
    });
};

// ***************************************************************************
// DISTANCIA
// ***************************************************************************

// INSERT
controller.insertarDistancia = async (req, res) => {
    const { distancia } = req.body
    req.getConnection(async (err, conn) => {
        if (err) throw (err)
        const insertarSql = "INSERT INTO distancia(distancia,fecha) values (?, now())"
        const insertar_query = mysql.format(insertarSql, [distancia])
        await conn.query(insertar_query, (err, result) => {
            if (err) {
                res.status(500).send("No se ha podido registrar la distancia")
                return;
            }
            res.status(200).send("Distancia registrada")
        })
    })
}

// SELECT
controller.obtenerDistancia = async (req, res) => {
    req.getConnection((err, conn) => {
        conn.query('SELECT * FROM distancia', (err, distancia) => {
            if (err) {
                res.status(404).json(err)
                return;
            }
            res.status(200).json(distancia);
        });
    });
};

// ***************************************************************************
// ILUMINACION
// ***************************************************************************

// INSERT
controller.insertarIluminacion = async (req, res) => {
    const { iluminacion } = req.body
    req.getConnection(async (err, conn) => {
        if (err) throw (err)
        const insertarSql = "INSERT INTO iluminacion(iluminacion,fecha) values (?, now())"
        const insertar_query = mysql.format(insertarSql, [iluminacion])
        await conn.query(insertar_query, (err, result) => {
            if (err) {
                res.status(500).send("No se ha podido registrar la iluminacion")
                return;
            }
            res.status(200).send("Iluminacion registrada")
        })
    })
}

// SELECT
controller.obtenerIluminacion = async (req, res) => {
    req.getConnection((err, conn) => {
        conn.query('SELECT * FROM iluminacion', (err, iluminacion) => {
            if (err) {
                res.status(404).json(err)
                return;
            }
            res.status(200).json(iluminacion);
        });
    });
};

// ***************************************************************************
// AIRE
// ***************************************************************************

// INSERT
controller.insertarAire = async (req, res) => {
    const { aire } = req.body
    req.getConnection(async (err, conn) => {
        if (err) throw (err)
        const insertarSql = "INSERT INTO aire(aire,fecha) values (?, now())"
        const insertar_query = mysql.format(insertarSql, [aire])
        await conn.query(insertar_query, (err, result) => {
            if (err) {
                res.status(500).send("No se ha podido registrar el aire")
                return;
            }
            res.status(200).send("Aire registrado")
        })
    })
}

// SELECT
controller.obtenerAire = async (req, res) => {
    req.getConnection((err, conn) => {
        conn.query('SELECT * FROM aire', (err, aire) => {
            if (err) {
                res.status(404).json(err)
                return;
            }
            res.status(200).json(aire);
        });
    });
};

// ***************************************************************************
// HUMEDAD
// ***************************************************************************

// INSERT
controller.insertarHumedad = async (req, res) => {
    const { humedad } = req.body
    req.getConnection(async (err, conn) => {
        if (err) throw (err)
        const insertarSql = "INSERT INTO humedad(humedad,fecha) values (?, now())"
        const insertar_query = mysql.format(insertarSql, [humedad])
        await conn.query(insertar_query, (err, result) => {
            if (err) {
                res.status(500).send("No se ha podido registrar la humedad")
                return;
            }
            res.status(200).send("Humedad registrada")
        })
    })
}

// SELECT
controller.obtenerHumedad = async (req, res) => {
    req.getConnection((err, conn) => {
        conn.query('SELECT * FROM humedad', (err, humedad) => {
            if (err) {
                res.status(404).json(err)
                return;
            }
            res.status(200).json(humedad);
        });
    });
};




module.exports = controller;