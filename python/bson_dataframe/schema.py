DATA = 'd'
MASK = 'm'
TYPE = 't'
PARAM = 'p'

# binary/string/list
OFFSET = 'o'

# list
LENGTH = 'l'

# struct
NAME = 'n'
FIELDS = 'f'

# dictionary
INDEX = 'i'
DICT = 'd'

CANONICAL_JSON_SCHEMA = {
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "DataFrame",
    "description": "DataFrame BSON serializer schema",
    "type": "object",
    "definitions": {
        "bson_binary": {
            "type": "object",
            "required": ["$binary"],
            "properites": {
                "$binary": {
                    "type": "object",
                    "required": ["base64", "subType"],
                    "properties": {
                        "base64": {
                            "type": "string"
                        },
                        "subType": {
                            "type": "string",
                            "const": "00"
                        }
                    }
                }
            }
        },
        "bson_numberInt": {
            "type": "object",
            "required": ["$numberInt"],
            "properties": {
                "$numberLong": {
                    "type": "string",
                    "pattern": "-?[0-9]+"
                }
            }
        },
        "bson_numberLong": {
            "type": "object",
            "required": ["$numberLong"],
            "properties": {
                "$numberLong": {
                    "type": "string",
                    "pattern": "-?[0-9]+"
                }
            }
        },
        "data_type": {
            "oneOf": [{
                "$ref": "#definitions/null_type"
            }, {
                "$ref": "#definitions/bool_type"
            }, {
                "$ref": "#definitions/signed_type"
            }, {
                "$ref": "#definitions/unsigned_type"
            }, {
                "$ref": "#definitions/float_type"
            }, {
                "$ref": "#definitions/datetime_type"
            }, {
                "$ref": "#definitions/timestamp_type"
            }, {
                "$ref": "#definitions/binary_type"
            }, {
                "$ref": "#definitions/opaque_type"
            }, {
                "$ref": "#definitions/dictionary_type"
            }, {
                "$ref": "#definitions/list_type"
            }, {
                "$ref": "#definitions/struct_type"
            }]
        },
        "null_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type": "string",
                    "const": "null"
                }
            }
        },
        "bool_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type": "string",
                    "const": "bool"
                }
            }
        },
        "signed_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type": "string",
                    "enum": ["int8", "int16", "int32", "int64"]
                }
            }
        },
        "unsigned_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type": "string",
                    "enum": ["uint8", "uint16", "uint32", "uint64"]
                }
            }
        },
        "float_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type": "string",
                    "enum": ["float16", "float32", "float64"]
                }
            }
        },
        "datetime_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type":
                    "string",
                    "enum": [
                        "date[d]", "date[ms]", "time[s]", "time[ms]",
                        "time[us]", "time[ns]"
                    ]
                }
            }
        },
        "timestamp_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type":
                    "string",
                    "enum": [
                        "timestamp[s]", "timestamp[ms]", "timestamp[us]",
                        "timestamp[ns]"
                    ]
                },
                "p": {
                    "type": "string",
                    "description": "Time zone"
                }
            }
        },
        "binary_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type": "string",
                    "enum": ["utf8", "bytes"]
                }
            }
        },
        "opaque_type": {
            "type": "object",
            "required": ["t", "p"],
            "properties": {
                "t": {
                    "type": "string",
                    "const": "opaque"
                },
                "p": {
                    "$ref": "#/definitions/bson_numberInt"
                }
            }
        },
        "dictionary_type": {
            "type": "object",
            "required": ["t"],
            "properties": {
                "t": {
                    "type": "string",
                    "enum": ["ordered", "factor"]
                },
                "p": {
                    "type": "object",
                    "required": ["i", "d"],
                    "properties": {
                        "i": {
                            "$ref": "#/definitions/signed_type"
                        },
                        "d": {
                            "$ref": "#/definitions/data_type"
                        }
                    }
                }
            }
        },
        "list_type": {
            "type": "object",
            "required": ["t", "p"],
            "properties": {
                "t": {
                    "type": "string",
                    "const": "list"
                },
                "p": {
                    "$ref": "#definitions/data_type"
                }
            }
        },
        "struct_type": {
            "type": "object",
            "required": ["t", "p"],
            "properties": {
                "t": {
                    "type": "string",
                    "const": "struct"
                },
                "p": {
                    "type": "array",
                    "minItems": 1,
                    "item": {
                        "type": "object",
                        "required": ["n", "t"],
                        "properties": {
                            "n": {
                                "type": "string"
                            },
                            "t": {
                                "$ref": "#definitions/data_type"
                            }
                        }
                    }
                }
            }
        },
        "array": {
            "oneOf": [{
                "$ref": "#definitions/null"
            }, {
                "$ref": "#definitions/bool"
            }, {
                "$ref": "#definitions/signed"
            }, {
                "$ref": "#definitions/unsigned"
            }, {
                "$ref": "#definitions/float"
            }, {
                "$ref": "#definitions/datetime"
            }, {
                "$ref": "#definitions/timestamp"
            }, {
                "$ref": "#definitions/binary"
            }, {
                "$ref": "#definitions/opaque"
            }, {
                "$ref": "#definitions/dictionary"
            }, {
                "$ref": "#definitions/list"
            }, {
                "$ref": "#definitions/struct"
            }]
        },
        "null": {
            "type": "object",
            "required": ["d", "m"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_numberLong"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "const": "null"
                }
            }
        },
        "bool": {
            "type": "object",
            "required": ["d", "m", "t"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_binary"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "const": "bool"
                }
            }
        },
        "signed": {
            "type": "object",
            "required": ["d", "m", "t"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_binary"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "enum": ["int8", "int16", "int32", "int64"]
                }
            }
        },
        "unsigned": {
            "type": "object",
            "required": ["d", "m", "t"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_binary"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "enum": ["uint8", "uint16", "uint32", "uint64"]
                }
            }
        },
        "float": {
            "type": "object",
            "required": ["d", "m", "t"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_binary"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "enum": ["float16", "float32", "float64"]
                }
            }
        },
        "datetime": {
            "type": "object",
            "required": ["d", "m", "t"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_binary"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type":
                    "string",
                    "enum": [
                        "date[d]", "date[ms]", "time[s]", "time[ms]",
                        "time[us]", "time[ns]"
                    ]
                }
            }
        },
        "timestamp": {
            "type": "object",
            "required": ["d", "m", "t"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_binary"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type":
                    "string",
                    "enum": [
                        "timestamp[s]", "timestamp[ms]", "timestamp[us]",
                        "timestamp[ns]"
                    ]
                },
                "p": {
                    "type": "string",
                    "description": "Time zone"
                }
            }
        },
        "binary": {
            "type": "object",
            "required": ["d", "m", "t", "o"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_binary"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "o": {
                    "description": "Offset array of int32",
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "enum": ["utf8", "bytes"]
                }
            }
        },
        "opaque": {
            "type": "object",
            "required": ["d", "m", "t", "p"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/bson_binary"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "const": "opaque"
                },
                "p": {
                    "$ref": "#/definitions/bson_numberInt"
                }
            }
        },
        "dictionary": {
            "type": "object",
            "required": ["d", "m", "t"],
            "properties": {
                "d": {
                    "type": "object",
                    "required": ["i", "d"],
                    "properties": {
                        "i": {
                            "$ref": "#/definitions/signed"
                        },
                        "d": {
                            "$ref": "#/definitions/array"
                        }
                    }
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "enum": ["factor", "ordered"]
                }
            }
        },
        "list": {
            "type": "object",
            "required": ["d", "m", "t", "p", "o"],
            "properties": {
                "d": {
                    "$ref": "#/definitions/array"
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "const": "list"
                },
                "p": {
                    "$ref": "#/definitions/data_type"
                },
                "o": {
                    "description": "Offset array of int32",
                    "$ref": "#/definitions/bson_binary"
                }
            }
        },
        "struct": {
            "type": "object",
            "required": ["d", "m", "t", "p"],
            "properties": {
                "d": {
                    "type": "object",
                    "required": ["l", "f"],
                    "properties": {
                        "l": {
                            "$ref": "#definitions/bson_numberLong"
                        },
                        "f": {
                            "type": "object",
                            "minProperties": 1,
                            "patternProperties": {
                                ".*": {
                                    "$ref": "#definitions/array"
                                }
                            }
                        }
                    }
                },
                "m": {
                    "$ref": "#/definitions/bson_binary"
                },
                "t": {
                    "type": "string",
                    "const": "struct"
                },
                "p": {
                    "type": "array",
                    "minItems": 1,
                    "item": {
                        "type": "object",
                        "required": ["n", "t"],
                        "properties": {
                            "n": {
                                "type": "string"
                            },
                            "t": {
                                "$ref": "#/definitions/data_type"
                            }
                        }
                    }
                }
            }
        }
    },
    "minProperties": 1,
    "patternProperties": {
        ".*": {
            "$ref": "#definitions/array"
        }
    }
}
