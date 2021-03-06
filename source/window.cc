//  Copyright (C) 2020 David Jatczak <david.j.361@gmail.com>
//  Copyright (C) 2020 Cameron Canning-Potter <cameroncanningpotter@cmail.carleton.ca>
//  Copyright (C) 2020 Pravdeep Deol
//  
//  This file is part of Paint Sheet Generator.
//  
//  Paint Sheet Generator is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  Paint Sheet Generator is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with Paint Sheet Generator.  If not, see <https://www.gnu.org/licenses/>.


#include <opencv2/highgui.hpp>
#include "../headers/window.h"
#include "../headers/helper.h"

Window::Window(Mat& input) : k(3), kmax(10), img(input), title("Paint Sheet Generator"), ps(img, k, title) {
	type = PROD;
	trackbarName = "k-value";
}

void Window::onMouse(int event, int x, int y, int flags, void* userdata) {
}

void Window::onTrackbar(int i, void* ptr) {
	Window* that = (Window*)ptr;
	that->realTrack(i);
}

void Window::realTrack(int i) {
	if (i < 2) {
		k = 2;
		setTrackbarPos(trackbarName, title, k);
		return;
	}
}


void Window::draw(bool recalculate, bool dial8) {
	if (recalculate)
		ps = PaintSheet(img, k, title, dial8);
	switch (type) {
	case ORIG:
		imshow(title, ps.orig);
		break;
	case QUANT:
		imshow(title, ps.quantized);
		break;
	case QUANT_NOE:
		imshow(title, ps.quantizedNoEdges);
		break;
	case PROD:
		imshow(title, ps.product);
		break;
	}
}

void Window::showOriginal() {
	this->type = ORIG;
	this->draw();
}

void Window::showQuantized() {
	this->type = QUANT;
	this->draw();
}

void Window::showQuantizedNoEdges() {
	this->type = QUANT_NOE;
	this->draw();
}

void Window::showProduct() {
	this->type = PROD;
	this->draw();
}

void Window::doRun(bool dial8) {
	cout << "Calculating..." << endl;
	this->draw(true, dial8);
	cout << "Finished calculating" << endl;
}

void Window::doSave() {
	imwrite("paint_sheet.jpg", ps.product);
	cout << "Paint sheet saved" << endl;
}

void Window::run() {
	namedWindow(title, WINDOW_AUTOSIZE);
	// Need a mouseCallBack so trackbar doesn't lag out
	setMouseCallback(title, onMouse, NULL);
	createTrackbar(trackbarName, title, &k, kmax, onTrackbar, this);
	draw(0);
	while (true) {
		int k = waitKey(10);
		switch (k) {
			case 27:          // Escape
				return;
			case 52:          // 4
				showOriginal();   
				break;
			case 51:		  // 3
				showQuantizedNoEdges();  
				break;
			case 50:		  // 2
				showQuantized();  
				break;
			case 49:          // 1
				showProduct();    
				break;
			case 101:          // e
				doRun(false);
				break;
			case 100:          // d
				doRun(true);
				break;
			case 115:         // s
				doSave();	
				break;
		}
		 
	}
}
