#include <Rcpp.h>
#include "PajeUnity.h"

using namespace Rcpp;

static CharacterVector nature;
static CharacterVector container;
static CharacterVector type;
static DoubleVector start;
static DoubleVector end;
static DoubleVector duration;
static DoubleVector value;

void _add_variable_entity (PajeEntity *e)
{
  nature.push_back("Variable");
  container.push_back(e->container()? e->container()->name() : "NULL");
  type.push_back(e->type()? e->type()->name() : "NULL");
  start.push_back(e->startTime());
  end.push_back(e->endTime());
  duration.push_back(e->duration());
  value.push_back(e->doubleValue());
}

DataFrame _collect_variable_dataframe (void)
{
  return DataFrame::create(Named("Nature") = nature,
			   Named("Container") = container,
			   Named("Type") = type,
			   Named("Start") = start,
			   Named("End") = end,
			   Named("Duration") = duration,
			   Named("Value") = value);
}
