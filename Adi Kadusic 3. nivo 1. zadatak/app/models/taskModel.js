const mongoose = require('mongoose')

const taskSchema = mongoose.Schema(
    {
        name: {
            type: String,
            required: [true, "Molimo vas unesite ime zadatka"]
        },
        due_date: {
            type: Date,
            required: true
        },
        description: {
            type: String,
            required: true
        }
    },
    {
        timestamps: true
    }
)

const Task = mongoose.model('Task', taskSchema)

module.exports = Task;