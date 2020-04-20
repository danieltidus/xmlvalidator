const validator = require('./');

const myArgs = process.argv.slice(2);

for (let index = 0; index < 3; index) {
  const retorno = validator.validate(myArgs[0],myArgs[1])
  
}

