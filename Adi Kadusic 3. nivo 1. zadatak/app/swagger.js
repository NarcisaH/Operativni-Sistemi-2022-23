const options = {
    definition: {
        openapi: '3.0.0',
        info: {
            title: 'Task manager REST API',
            version: '1.0.0',
            description: 'Jednostavni CRUD REST API'
        },
        servers: [
            {
                url: 'http://localhost:3000',
                description: 'Testni server'
            }
        ],
        components: {
            schemas: {
                Task: {
                    type: 'object',
                    required: ['name','due_date','description'],
                    properties: {
                        name: {
                            type: 'string',
                            description: 'Ime zadatka'
                        },
                        due_date: {
                            type: 'string',
                            format: 'date',
                            description: 'Datum do kojeg se zadatak treba obaviti'
                        },
                        description: {
                            type: 'string',
                            description: 'Opis zadatka'
                        }
                    }
                }
            },
            responses: {
                500: {
                    description: 'Error u zahtjevu',
                    contents: 'application/json'
                }
            }
        }
    },
    apis: ['./server.js','./models/taskModel.js']
}

module.exports = options