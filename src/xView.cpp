#include "ofxVui.h"
#include "xView.h"

namespace VUI {
	View::~View() {};
	View::View() {};

	void View::Done() {
		VUI::Next();
	}
}
