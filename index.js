// import native addon
const addon = require('bindings')('xmlvalidator');

// expose module API
exports.validate = addon.validate;
