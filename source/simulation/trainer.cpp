/*
The MIT License (MIT)

Copyright (c) 2006-2016 Thomas Fussell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "simulation/trainer.hpp"
#include "brain/brain.hpp"
#include "interface/script.hpp"
#include "simulation/entity.hpp"

namespace cort {

trainer::trainer(const std::string &filename, brain * /*ent*/) : script_(filename) {};/* :
    m_CurrentTime(0),
    m_DeltaTime(0),
	m_WaitStart(0.),
	m_WaitDuration(0.),
	m_Waiting(false),
	m_CurrentRepeats(0),
	m_NumRepeats(0),
	m_TimeBtStartMsgAndInput(0.),
	m_TimeToWaitForTestResponse(0.),
	m_TimeBtOutputAndEndMsg(0.),
	m_TimeBtTrainInAndOut(0.),
	m_TimeBtTrainIterations(0.),
	m_TimeBtTestIterations(0.),
	m_TotalTrainingIterations(0),
	m_TotalTestingIterations(0),
	m_CurrTrainingIteration(0),
	m_CurrTestingIteration(0),
    m_ID(0),
	m_Input(""),
	m_ExpectedOutput(""),
	m_BciReward(nullptr),
	m_BciInput(nullptr),
	m_BciOutput(nullptr),
	m_BciOutputTrainer(nullptr),
	m_Script(filename),
	m_State(TeachStartMsg)
{
	std::string inputCommandsFile = m_Script.GetNextString();
	std::string outputCommandsFile = m_Script.GetNextString();

	m_BciInput = new BCI_SpeechFromFile(ent->get_layer("input"), inputCommandsFile);
	m_BciReward = new BCI_RewardPunishment(ent->get_layer("output"));
	m_BciOutput = new BCI_SpeechFromFile(ent->get_layer("output"), outputCommandsFile);
	m_BciOutputTrainer = new BCI_SpeechFromFile(ent->get_layer("outputTrainer"), outputCommandsFile);

	m_TimeBtTrainInAndOut = m_Script.GetNextDouble();
	m_TimeBtTrainIterations = m_Script.GetNextDouble();
	m_TimeBtTestIterations = m_Script.GetNextDouble();
	m_TotalTrainingIterations = m_Script.GetNextInt();
	m_TotalTestingIterations = m_Script.GetNextInt();
	m_TimeBtStartMsgAndInput = m_Script.GetNextDouble();
	m_TimeToWaitForTestResponse = m_Script.GetNextDouble();
	m_TimeBtOutputAndEndMsg = m_Script.GetNextDouble();

	m_Input = m_Script.GetNextString();
	m_ExpectedOutput = m_Script.GetNextString();
	m_NumRepeats = m_Script.GetNextInt();

	Wait(m_TimeBtTrainIterations);
}
*/

trainer::~trainer()
{
	if(bci_input_)
		delete bci_input_;

	if(bci_output_)
		delete bci_output_;

	if(bci_output_trainer_)
		delete bci_output_trainer_;
}

void trainer::wait(double duration)
{
	waiting_ = true;
	wait_start_ = current_time_;
	wait_duration_ = duration;
}

bool trainer::is_completed()
{
	return state_ == training_state::completed;
}

//I guess this is a finite state machine?
//Maybe there's a better way to do it, but this made sense
void trainer::update(double /*time*/)
{
/*
	double last = m_CurrentTime;

	m_CurrentTime = time;
	m_DeltaTime = m_CurrentTime - last;

	std::string output = "";

	if(m_Waiting)
	{
		if(m_State == TestWaitForOutput)
		{
			output = m_BciOutput->get_state();

			if(output != "")
			{
				m_Waiting = false;
			}
		}

		if(m_CurrentTime >= m_WaitStart + m_WaitDuration)
		{
			m_Waiting = false;
		}
	}

	if(!m_Waiting)
	{
		switch(m_State)
		{
		case TeachStartMsg:
			{
				//m_BciInput->SetState("StartTeach");
				m_State = TeachSetInput;

				Wait(m_TimeBtStartMsgAndInput);

				break;
			}

		case TeachSetInput:
			{
				m_BciInput->set_state(m_Input);
				std::printf("Taught Input: %s\n", m_Input.c_str());
				m_State = TeachSetOutput;

				Wait(m_TimeBtTrainInAndOut);

				break;
			}

		case TeachSetOutput:
			{
				m_BciOutputTrainer->set_state(m_ExpectedOutput);
				std::printf("Taught Output: %s\n", m_ExpectedOutput.c_str());
				m_State = TeachEndMsg;

				Wait(m_TimeBtOutputAndEndMsg);

				break;
			}

		case TeachEndMsg:
			{
				//m_BciInput->set_state("EndTeach");

				if(++m_CurrentRepeats == m_NumRepeats)
				{	
					if(++m_CurrTrainingIteration == m_TotalTrainingIterations)
					{
						m_State = TestStartMsg;
						m_CurrTestingIteration = 0;

						Wait(m_TimeBtTestIterations);

						break;
					}

					m_Input = m_Script.GetNextString();
					m_ExpectedOutput = m_Script.GetNextString();
					m_NumRepeats = m_Script.GetNextInt();

					m_CurrentRepeats = 0;
				}

				m_State = TeachStartMsg;

				Wait(m_TimeBtTrainIterations);

				break;
			}

		case TestStartMsg:
			{
				//m_BciInput->set_state("StartTest");
				m_State = TestSetInput;

				Wait(m_TimeBtStartMsgAndInput);
				break;
			}

		case TestSetInput:
			{
				m_Input = m_Script.GetNextString();
				m_ExpectedOutput = m_Script.GetNextString();

				m_BciInput->set_state(m_Input);
				std::printf("Testing Input: %s\n", m_Input.c_str());

				m_State = TestWaitForOutput;

				Wait(m_TimeToWaitForTestResponse);

				break;
			}

		case TestWaitForOutput:
			{
				std::printf("Received Response: %s (Correct Response: %s)\n", output.c_str(),m_ExpectedOutput.c_str());
				
				m_BciOutput->get_neuron(m_ExpectedOutput)->BackpropagateUp();
				if(output != "" && output != m_ExpectedOutput)
				{
					m_BciOutput->GetNeuron(output)->BackpropagateDown();
					std::printf("Bad\n");
					//m_BciReward->set_state("Good");
				}
				else
				{
					std::printf("Good\n");
				}

				if(output == m_ExpectedOutput)
				{
					m_BciOutput->GetNeuron(output)->BackpropagateUp();
					std::printf("Good\n");
					//m_BciReward->set_state("Good");
				}
				else
				{
					if(output != "")
						m_BciOutput->GetNeuron(output)->BackpropagateDown();
					else
					{
						for(std::map<std::string, Synapse *>::iterator i = m_BciOutput->m_SynapseCommandMap.begin(); i != m_BciOutput->m_SynapseCommandMap.end(); i++)
						{
							m_BciOutput->GetNeuron(i->first)->BackpropagateUp();
						}
					}
					std::printf("Bad\n");
					//m_BciReward->set_state("Bad");
				}

				m_State = TestEndMsg;

				Wait(m_TimeBtOutputAndEndMsg);

				break;
			}

		case TestEndMsg:
			{
				//m_BciInput->set_state("EndTest");

				if(++m_CurrTestingIteration == m_TotalTestingIterations)
				{
					std::printf("Trainer Finished\n");
					m_State = Finished;

					Wait(m_TimeBtTestIterations);

					break;
				}

				m_State = TestStartMsg;

				Wait(m_TimeBtTestIterations);

				break;
			}

		case Finished:
		default:
			{
				break;
			}
		}
	}
*/
}

layer *trainer::get_layer(const std::string &name)
{
	if(name == "input")
	{
		return bci_input_->get_layer();
	}
	else if(name == "output")
	{
		return bci_output_->get_layer();
	}
	else if(name == "outputTrainer")
	{
		return bci_output_trainer_->get_layer();
	}

	return 0;
}

void trainer::reset()
{

}

void trainer::print_state()
{

}

} // namespace cort
