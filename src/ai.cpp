#include <iostream>
#include <cmath>
#include <algorithm>
#include "neuron.hpp"
#include "game.hpp"
#include "utils.hpp"

#include <cmath>

using namespace std;

double delta(double a, double b) {
	return (a - b) * b * (1 - b);
}

void Game::ai_learn() {
	double input[16];

	for (int k = 0; k < m_grid_result.size(); k++) {

		int target[4] = { 0,0,0,0 };

		switch (result[k]) {
		case direction::NORTH:
			target[0] = 1;
		case direction::SOUTH:
			target[1] = 1;
		case direction::EAST:
			target[2] = 1;
		case direction::WEST:
			target[3] = 1;
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				input[i * 4 + j] = m_grid_result[k].gridmap[i][j]; // starts at left top, ends at right bottom
			}
		}

		ilayer.processlayer(input);
		hlayer1.processlayer(ilayer.outputdata);
		hlayer2.processlayer(hlayer1.outputdata);
		hlayer3.processlayer(hlayer2.outputdata);
		olayer.processlayer(hlayer3.outputdata);

		/*for (int i = 0; i < 4; i++) {
			int max = 0; int index=0;
			if (max < olayer.outputdata[i]) {
				max = olayer.outputdata[i]; index = i;
			}
		}*/

		double od[4];
		double d3[16];
		double d2[16];
		double d1[16];



		//olayer.neuron[0].input_weight[0] = olayer.neuron[0].input_weight[0] + 0.1 * delta(target[0], olayer.outputdata[0], hlayer1.outputdata[0]);
			//(target[0]-olayer.outputdata[0])* olayer.outputdata[0]*(1- olayer.outputdata[0])*
		for (int i = 0; i < 4; i++) {
			od[i] = delta(target[i], olayer.outputdata[i]);
			for (int j = 0; j < 16; j++) {
				olayer.neuron[i].input_weight[j] = olayer.neuron[i].input_weight[j] + 0.1 * od[i] * hlayer3.outputdata[j];
			}
		}
		//hlayer1.neuron[0].input_weight[0] = hlayer1.neuron[0].input_weight[0] + 0.1 * delta(target[0], olayer.outputdata[0], hlayer1.outputdata[0]);
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				double temp = 0;
				for (int n = 0; n < 4; n++) temp = temp + olayer.neuron[n].input_weight[i] * od[n];
				d3[i] = temp * hlayer3.outputdata[i] * (1 - hlayer3.outputdata[i]);
				hlayer3.neuron[i].input_weight[j] = hlayer3.neuron[i].input_weight[j] + 0.1 + temp * hlayer3.outputdata[i] * (1 - hlayer3.outputdata[i]) * hlayer2.outputdata[j];
			}
		}
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				double temp = 0;
				for (int n = 0; n < 16; n++) temp = temp + hlayer3.neuron[n].input_weight[i] * d3[n];
				d2[i] = temp * hlayer2.outputdata[i] * (1 - hlayer2.outputdata[i]);
				hlayer2.neuron[i].input_weight[j] = hlayer2.neuron[i].input_weight[j] + 0.1 + temp * hlayer2.outputdata[i] * (1 - hlayer2.outputdata[i]) * hlayer1.outputdata[j];
			}
		}
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				double temp = 0;
				for (int n = 0; n < 16; n++) temp = temp + hlayer2.neuron[n].input_weight[i] * d2[n];
				d1[i] = temp * hlayer1.outputdata[i] * (1 - hlayer1.outputdata[i]);
				hlayer1.neuron[i].input_weight[j] = hlayer1.neuron[i].input_weight[j] + 0.1 + temp * hlayer1.outputdata[i] * (1 - hlayer1.outputdata[i]) * ilayer.outputdata[j];
			}
		}
	}

}



void Game::search() {

	direction a;

	for (int i = 0; i < 100; ++i)
	{
		Game g();
		vector <direction> d1;

		while (state_!=GameState::LOST)
		{
			a = rand_action();
			switch (a) {
			case direction::WEST:
				shiftLeft();
				mergeLeft();
				shiftLeft();

				x_axis_ = true;
				break;
			case direction::EAST:
				shiftRight();
				mergeRight();
				shiftRight();

				x_axis_ = true;
				break;
			case direction::NORTH:
				shiftUp();
				mergeUp();
				shiftUp();

				x_axis_ = false;
				break;
			case direction::SOUTH:
				shiftDown();
				mergeDown();
				shiftDown();

				x_axis_ = false;
				break;

			d1.push_back(a);
			gridtemp m;
			copy(&tiles_[0][0], &tiles_[0][0] + 16, &m.gridmap[0][0]);
			m_grid_temp.push_back(m);
		}
		d.push_back(d1);
		m_grid_rec.push_back(m_grid_temp);
	}
	int max = 0;
	for (int i = 0; i < 100; i++) {
		int sz = 0;
		if (sz < d[i].size()) {
			sz = d[i].size();
			max = i;
		}
	}
	copy(d[max].begin(), d[max].end(), result);
	copy(m_grid_rec[max].begin(), m_grid_rec[max].end(), m_grid_result);
}