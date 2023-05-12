const express = require('express')
const app = express()
const mongoose = require('mongoose')
const Task = require('./models/taskModel')
const swaggerJSDoc = require('swagger-jsdoc')
const swaggerUi = require('swagger-ui-express')
const options = require('./swagger')
const cors = require('cors')

// Middleware
app.use(express.json())
app.use(express.urlencoded({extended: false}))
app.use(cors())

/**
 * @swagger
 * tags:
 *   name: Tasks
 *   description: API endpoints za task manager
 */


app.get('/', (req, res) => {
    res.send("Node API")
})


/**
 * @swagger
 *   /tasks:
 *     get:
 *       summary: Procitaj sve zadatke
 *       tags: [Tasks]
 *       responses:
 *         "200":
 *           description: Lista svih zadataka
 *           contents:
 *             application/json::
 *               schema:
 *                 $ref: '#/components/schemas/Task'
 *         "500":
 *           $ref: '#/components/responses/500'
 */
app.get('/tasks', async(req,res) => {
    try {
        const tasks = await Task.find({})
        res.status(200).json(tasks)
    } catch (error) {
        res.status(500).json({message: error.message})
    }
})

/**
 * @swagger
 *   /tasks/{taskId}:
 *     get:
 *       summary: Procitaj zadatak pomocu ID-a
 *       tags: [Tasks]
 *       parameters:
 *         - in: path
 *           name: taskId
 *           schema:
 *             type: string
 *           required: true
 *           description: id zadatka
 *       responses:
 *         "200":
 *           description: Lista svih zadataka
 *           contents:
 *             application/json::
 *               schema:
 *                 $ref: '#/components/schemas/Task'
 *         "500":
 *           $ref: '#/components/responses/500'
 */
app.get('/tasks/:id', async(req, res) => {
    try {
        const {id} = req.params
        const task = await Task.findById(id)
        res.status(200).json(task)
    } catch (error) {
        res.status(500).json({message: error.message})
    }
})

/**
 * @swagger
 * /tasks/{taskId}:
 *   put:
 *     summary: Ažuriraj zadatak
 *     tags: [Tasks]
 *     parameters:
 *       - name: taskId
 *         in: path
 *         description: ID zadatka za ažuriranje
 *         required: true
 *         schema:
 *           type: string
 *     requestBody:
 *       required: true
 *       content:
 *         application/json:
 *           schema:
 *             $ref: '#/components/schemas/Task'
 *     responses:
 *       200:
 *         description: Zadatak uspješno ažuriran
 *         content:
 *           application/json:
 *             schema:
 *               $ref: '#/components/schemas/Task'
 *       500:
 *         $ref: '#/components/responses/500'
 */
app.put('/tasks/:id', async(req, res) => {
    try {
        const {id} = req.params
        const task = await Task.findByIdAndUpdate(id, req.body)  
        if(!task) {
            return res.status(404).json({message: `Ne mozemo naci zadatak sa ID-om ${id}`})
        }
        const updatedTask = await Task.findById(id)
        res.status(200).json(task)
    } catch (error) {
        res.status(500).json({message: error.message})
    }
})

/**
 * @swagger
 * /tasks/{taskId}:
 *   delete:
 *     summary: Izbrisi zadatak sa zadanim ID-om
 *     tags: [Tasks]
 *     parameters:
 *       - in: path
 *         name: taskId
 *         schema:
 *           type: string
 *         required: true
 *         description: ID zadatka za brisanje
 *     responses:
 *       204:
 *         description: Zadatak uspješno izbrisan
 *       500:
 *         $ref: '#/components/responses/500'
 */
app.delete('/tasks/:id', async(req, res) => {
    try {
        const {id} = req.params
        const task = await Task.findByIdAndDelete(id)
        if(!task) {
            return res.status(404).json({message: `Ne mozemo naci zadatak sa ID-om ${id}`})
        }
        res.status(200).json(task)
    } catch (error) {
        res.status(500).json({message: error.message})
    }
})


/**
 * @swagger
 *   /tasks:
 *     post:
 *       summary: Kreiraj zadatak
 *       tags: [Tasks]
 *       requestBody:
 *           required: true
 *           content:
 *             application/json:
 *               schema:
 *                 $ref: '#/components/schemas/Task'
 *       responses:
 *         "200":
 *           description: Zadatak uspjesno kreiran
 *           contents:
 *             application/json:
 *         "500":
 *           $ref: '#/components/responses/500'
 */
app.post('/tasks', async(req, res) => {
    try {
        const task = await Task.create(req.body)
        res.status(200).json(task)
    } catch (error) {
        console.log(error.message)
        res.status(500).json({message: error.message})
    }
})


const specs = swaggerJSDoc(options)
app.use('/api-docs', swaggerUi.serve, swaggerUi.setup(specs))

app.listen(3000, () => {
    console.log("Task manager aplikacija pokrenuta na portu 3000!")
})

mongoose.
connect('mongodb+srv://admin:root@database.vydefxe.mongodb.net/Node-API?retryWrites=true&w=majority')
.then(() => {
    console.log('Spojeno na MongoDB!')
}).catch((error) => {
    console.log(error)
})


