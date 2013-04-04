#include "Universe.h"
#include "Object.h"

using namespace mcr;

const Event Object::ComAdded, Object::ComRemoved;
const EventArray Object::Events(Object::ComAdded, Object::ComRemoved);
