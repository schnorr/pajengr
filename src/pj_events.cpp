#include <Rcpp.h>
#include "PajeUnity.h"

using namespace Rcpp;

static CharacterVector nature;
static CharacterVector container;
static CharacterVector type;
static DoubleVector start;
static CharacterVector value;

void _add_event_entity (PajeEntity *e)
{
  nature.push_back("Variable");
  container.push_back(e->container()? e->container()->name() : "NULL");
  type.push_back(e->type()? e->type()->name() : "NULL");
  start.push_back(e->startTime());
  value.push_back(e->value()? e->value()->name() : "NULL");
}

DataFrame _collect_event_dataframe (void)
{
  return DataFrame::create(Named("Nature") = nature,
			   Named("Container") = container,
			   Named("Type") = type,
			   Named("Start") = start,
			   Named("Value") = value);
}
