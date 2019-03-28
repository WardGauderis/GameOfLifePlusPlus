//============================================================================
// @name        : exporter.h
// @author      : Thomas Dooms
// @date        : 3/28/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#pragma once

#include "../automata/fa.h"
#include "../automata/pda.h"
#include "../automata/tm.h"
#include "json/json.h"

using nlohmann::json;

enum Type{html, json, dot, png};

class Exporter
{
public:
    void ExportFA (const FA&  fa , Type type, const std::string& path);
    void ExportPDA(const PDA& pda, Type type, const std::string& path);
    void ExportTM (const TM&  tm , Type type, const std::string& path);
private:
};

// here we can write the code to export to json/dot/html...