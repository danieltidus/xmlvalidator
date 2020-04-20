#include <napi.h>
#include <string>
#include <vector>

#include "validate.h"


// native C++ function that is assigned to `greetHello` property on `exports` object
Napi::Array validate(const Napi::CallbackInfo& info) {
    
    // call `validate` function from `validate.cpp` file
    std::string xsdPath = (std::string) info[0].ToString();
    std::string xmlPath = (std::string) info[1].ToString();
    std::vector<std::string> *result = _validate(xsdPath, xmlPath);

    //Copy array and delete
    Napi::Array errors = Napi::Array::New(info.Env(), result->size());
    
    for (size_t i = 0; i < result->size(); i++)
    {
        errors.Set(i, result->at(i));
    }

    //Clear
    result->clear();
    delete result;
    // return new `Napi::String` value
    return errors;
}

// callback method when module is registered with Node.js
Napi::Object Init(Napi::Env env, Napi::Object exports) {

    // set a key on `exports` object   
    exports.Set(
        Napi::String::New(env, "validate"),
        Napi::Function::New(env, validate) 
    );

    // return `exports` object (always)
    return exports;
}

// register `xmlvalidator` module which calls `Init` method
NODE_API_MODULE(xmlvalidator, Init)
