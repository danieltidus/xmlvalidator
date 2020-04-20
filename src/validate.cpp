#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <vector>

#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/xmlschemas.h>

#include "validate.h"

u_int32_t get_file_size(const char *file_name) {
    struct stat buf;
    if ( stat(file_name, &buf) != 0 ) return(0);
    return (unsigned int)buf.st_size;
}

void handleValidationError(void *ctx, const char *format, ...) {
    char *errMsg;
    std::vector<std::string> *errorVector =  (std::vector<std::string>*)ctx;
    va_list args;
    va_start(args, format);
    vasprintf(&errMsg, format, args);
    std::string error(errMsg);
    errorVector->push_back(error);
    free(errMsg);
    va_end(args);
}

void clearData( xmlSchemaParserCtxtPtr parserCtxt,
                xmlSchemaPtr schema,
                xmlSchemaValidCtxtPtr validCtxt,
                xmlDocPtr xmlDocumentPointer = NULL

){
  if (parserCtxt) xmlSchemaFreeParserCtxt(parserCtxt);
  if (schema) xmlSchemaFree(schema);
  if (validCtxt) xmlSchemaFreeValidCtxt(validCtxt);
  if (xmlDocumentPointer) xmlFreeDoc(xmlDocumentPointer);

  xmlCleanupParser();
  
}

std::vector<std::string>* _validate(std::string xsdPath, std::string xmlPath){
  
  std::vector<std::string> *errors = new std::vector<std::string>();
  
  int xmlLength = get_file_size(xmlPath.c_str());

  if(xmlLength <= 0){
    errors->push_back("Invalid XML File");
    return errors;
  }
    

  char *xmlSource = (char *)malloc(sizeof(char) * xmlLength);

  FILE *p = fopen(xmlPath.c_str(), "r");
  char c;
  unsigned int i = 0;
  while ((c = fgetc(p)) != EOF) {
      xmlSource[i++] = c;
  }

  fclose(p);

  xmlSchemaParserCtxtPtr parserCtxt = NULL;
  xmlSchemaPtr schema = NULL;
  xmlSchemaValidCtxtPtr validCtxt = NULL;

  xmlDocPtr xmlDocumentPointer = xmlParseMemory(xmlSource, xmlLength);
  free(xmlSource);
  
  parserCtxt = xmlSchemaNewParserCtxt(xsdPath.c_str());

  if (parserCtxt == NULL) {
    clearData(parserCtxt, schema, validCtxt);
    errors->push_back("Could not create XSD schema parsing context.");
    return errors;
  }

  schema = xmlSchemaParse(parserCtxt);
  if (schema == NULL) {
    clearData(parserCtxt, schema, validCtxt);
    errors->push_back("Could not parse XSD schema.");
    return errors;
  }

  validCtxt = xmlSchemaNewValidCtxt(schema);

  if (!validCtxt) {
    clearData(parserCtxt, schema, validCtxt);
    errors->push_back("Could not create XSD schema validation context.");
    return errors;
  }
  
  xmlSetStructuredErrorFunc(NULL, NULL);
  xmlSetGenericErrorFunc(errors, handleValidationError);
  xmlThrDefSetStructuredErrorFunc(NULL, NULL);
  xmlThrDefSetGenericErrorFunc(errors, handleValidationError);

  xmlSchemaValidateDoc(validCtxt, xmlDocumentPointer);

  clearData(parserCtxt, schema, validCtxt, xmlDocumentPointer);
  return errors;
}
