#pragma once

#include "timer.h"
#include "generate.h"
#include "melodi.h"

#define NOTE_MNT 500

NOTE *sequns;
void init_seq() {
	sequns = reinterpret_cast<NOTE*>(data_piano);
}