#include "VCS.h"

int main() {
	VCS object("new.txt");
	object.save();
	object.list();
}