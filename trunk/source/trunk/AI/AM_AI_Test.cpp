/*
Zombie Project.  A game.  With Zombies.
Copyright (C) 2007  Matthew Chudleigh

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/** \file AM_AI_Test.cpp
* \brief This is the test file for the AM_AI library.
*/

#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

#include "AM_Math.hpp"

#include "AM_AI_Output.hpp"
#include "AM_AI_Action.hpp"
#include "AM_AI_Actor.hpp"
#include "AM_AI_Fuzzy.hpp"

namespace AutoMatic
{
	class Monkey_Action : public AI::Action
	{
	public:
		Monkey_Action(AI::Action::ID const & id_, AI::Brain::Ptr const & brain_)
			: AI::Action(id_, brain_) 
		{}

		/// 
		/** When the MonkeyAction is called, the conditions satisfied by the currentState
		*** (from the Brain StateHistory) are listed through validConditions.  The 
			* AI currentValue is then modified, i.e., the action is performed.

			* Note that 'performing' the action could be delayed (i.e., setting the 
			* state of the AI and the outcome for the currentState() actionOutcome 
			* could be performed after the perform() method exits).  Just store the 
			* Action's State pointer, and whatever else is needed to decide the change
			* in the Actor and ActionOutcome after some amount of time.
		*/
		virtual Action::Outcome perform(AI::Actor & ai_)
		{
			// Get the conditions satisfied by the state for this action (in this test, only 1 per action).
			std::vector<Condition::ID> condition = validConditions(*currentState());

			assert(condition.size() > 0);

			AI::Action::Outcome outcome = AI::Action::Impossible;
			float addToOutcome = 0.0f;
				
			// Copy the state the action was called from.
				// Then, perform the desired action and modify the state.
			// TODO: Should this just be done by default?
			ai_.currentState().setValues(*currentState());

			if(_id == "Move")
			{ // Monkey is moving.  It can be assumed that if this action is available, the monkey is NOT on the box.

				AI::Output::Out() << "Moving ";
				
				if(condition[0] == "ToBox")
				{ // Monkey is moving to the box (because it is currently away from the box).
					ai_.currentValue("MonkeyAndBox", "NextTo"); // The outcome state will have the monkey next to the box.
					addToOutcome = 1.0f;
					AI::Output::Out() << "to box." << AI::Output::endl();
				}
				else if(condition[0] == "FromBox")
				{ // Monkey is moving away from the box (because it is currently next to the box).
					ai_.currentValue("MonkeyAndBox", "Away"); // The outcome state will have the monkey away from the box.
					AI::Output::Out() << "away from box.  You stupid monkey." << AI::Output::endl();
				}
				else 
				{ // Whoops.
					AI::Output::Out() << " ** ERROR ** \n"; assert(false);
				}

				outcome = AI::Action::Completed;
			}
			else if(_id == "ClimbBox")
			{ // The monkey is climbing the box (up or down).  It can be assumed that the monkey is not away from the box.
				AI::Output::Out() << "Climbing ";

				if (condition[0] == "Up")
				{ // The ClimbBox Up condition has been satisfied
					AI::Output::Out() << " up box." << AI::Output::endl();
					ai_.currentValue("MonkeyAndBox", "OnBox");
				
					if(isCurrentValue("BoxUnderBannanas", "T"))
					{
						addToOutcome = 1.0f;
					}
				}
				else if (condition[0] == "Down")
				{ // The ClimbBox Down condition has been satisfied
					AI::Output::Out() << " down box." << AI::Output::endl();
					ai_.currentValue("MonkeyAndBox", "NextTo" );
				}
				else 
				{
					AI::Output::Out() << " ** ERROR ** \n"; assert(false);
				}
				
				outcome = AI::Action::Completed;
			}
			else if(_id == "PushBox")
			{
				AI::Output::Out() << "Pushing box ";

				if(condition[0] == "UnderBannanas")
				{ // The PushBox UnderBannanas condition has been satisfied
					addToOutcome = 1.0f;
					ai_.currentValue("BoxUnderBannanas", "T");
					AI::Output::Out() << "under the bananas." << AI::Output::endl();
				}
				else if(condition[0] == "NotUnder")
				{ // The PushBox NotUnder condition has been satisfied
					ai_.currentValue("BoxUnderBannanas", "F");
					AI::Output::Out() << "away from the bananas." << AI::Output::endl();
				}
				else 
				{
					AI::Output::Out() << " ** ERROR ** \n"; assert(false);
				}
				ai_.currentValue("MonkeyAndBox", "NextTo"); 
				outcome = AI::Action::Completed;
			}
			else if(_id == "TakeBannanas")
			{ 
				if(condition[0] == "Yum")
				{ // The TakeBannanas Yum condition has been satisfied
					AI::Output::Out() << "Yum Yum Bananas!" << AI::Output::endl();
					ai_.currentValue("HasBannanas", "T");
					outcome = AI::Action::Succeeded;
				}
				else
				{
					AI::Output::Out() << " ** ERROR ** \n"; assert(false);
				}
			}

			currentState()->addToOutcome(_id, addToOutcome);
			return outcome;
		}
	private:
		
	};

	void RunAITest(void)
	{
		AI::Brain::Ptr brain(new AI::Brain);

		// 1. Note that parts are implicitly created.
		// 2. Set up actions.
		// Set up Move action. 
		// Post conditions are NOT required, because they are only used to 
			// add influence to an action based on the possibility that they will lead to a goal.
		// TODO: Passing the brain to the action suggests the brain should
			// control creating the action...
			
		Monkey_Action move("Move", brain);

		// For an action to be available, the current actor state must have the same values for the parts listed.
		move.pre("ToBox", "MonkeyAndBox", "Away"); // Pre-Condition -> Part, Value
		move.post("ToBox", "MonkeyAndBox", "NextTo"); // Post-Condition -> Part, Value

		move.pre("FromBox", "MonkeyAndBox", "NextTo");
		move.post("FromBox", "MonkeyAndBox", "Away");

		// Set up Push action
		Monkey_Action pushBox("PushBox", brain);
		
		pushBox.pre("UnderBannanas", "MonkeyAndBox", "NextTo"); // Pre-Condition -> Part, Value
		pushBox.pre("UnderBannanas", "BoxUnderBannanas", "F");	 //								-> Part, Value
		pushBox.post("UnderBannanas", "BoxUnderBannanas", "T");

		pushBox.pre("NotUnder", "MonkeyAndBox", "NextTo");
		pushBox.pre("NotUnder", "BoxUnderBannanas", "T");
		pushBox.post("NotUnder", "BoxUnderBannanas", "F");

		// Set up Climb action
		Monkey_Action climbBox( "ClimbBox", brain );
		climbBox.pre("Up", "MonkeyAndBox", "NextTo");
		climbBox.post("Up", "MonkeyAndBox", "OnBox");

		climbBox.pre("Down", "MonkeyAndBox", "OnBox" );
		climbBox.post("Down", "MonkeyAndBox", "NextTo" );

		// Set up Take action
		Monkey_Action takeBannanas( "TakeBannanas", brain );
		takeBannanas.pre("Yum", "MonkeyAndBox", "OnBox" );
		takeBannanas.pre("Yum", "BoxUnderBannanas","T");
		takeBannanas.post("Yum", "HasBannanas", "T");

		// 3. Setup an actor.
		AI::Actor monkey(brain);
		
		// Give the actor actions it can perform.
		monkey.addAction( &move );
		monkey.addAction( &pushBox );
		monkey.addAction( &climbBox );
		monkey.addAction( &takeBannanas );

		const int ITERATIONS = 1;

		float avg = 0.0f;
		for ( int i = 0; i < ITERATIONS; ++i )
		{
			// Set the actor's current state part values.
			monkey.currentValue("BoxUnderBannanas", "F");
			monkey.currentValue("HasBannanas", "F");
			monkey.currentValue("MonkeyAndBox", "Away");

			// Set the actor's current goal.
			AI::Actor::Goal getBannanas;
			getBannanas.influence(10.0f);
			getBannanas.state().setValue(brain->part("HasBannanas"), "T");

			monkey.addGoal(getBannanas);
			
			int actions = 0;
			AI::Action::Outcome outcome = AI::Action::Impossible;
			do 
			{
				actions ++;
				outcome = monkey.getAction()->perform(monkey);
			} while(outcome != AI::Action::Impossible && monkey.goalsSatisfied().size() == 0);

			assert(monkey.goalsSatisfied().size() == 1);
			avg += actions;
		}
		
		AI::Output::Out() << "Actions: " << avg / ITERATIONS << AI::Output::endl();
	}

	void FuzzyAITest(void)
	{
		AI::Fuzzy::Logic health("Health");
		health.getSet("Bad")->add(AI::Fuzzy::Set::Member(0.0f,1.0f));
		health.getSet("Bad")->add(AI::Fuzzy::Set::Member(0.3f,1.0f));
		health.getSet("Bad")->add(AI::Fuzzy::Set::Member(0.6f,0.0f));

		health.getSet("Good")->add(AI::Fuzzy::Set::Member(0.4f,0.0f));
		health.getSet("Good")->add(AI::Fuzzy::Set::Member(0.7f,1.0f));
		health.getSet("Good")->add(AI::Fuzzy::Set::Member(1.0f,1.0f));

		AI::Fuzzy::Logic armour("Armour");
		armour.getSet("Inadequate")->add(AI::Fuzzy::Set::Member(0.0f, 1.0f));
		armour.getSet("Inadequate")->add(AI::Fuzzy::Set::Member(0.2f, 1.0f));
		armour.getSet("Inadequate")->add(AI::Fuzzy::Set::Member(0.4f, 0.0f));

		armour.getSet("Marginal")->add(AI::Fuzzy::Set::Member(0.3f, 0.0f));
		armour.getSet("Marginal")->add(AI::Fuzzy::Set::Member(0.5f, 1.0f));
		armour.getSet("Marginal")->add(AI::Fuzzy::Set::Member(0.7f, 0.0f));

		armour.getSet("Adequate")->add(AI::Fuzzy::Set::Member(0.6f, 0.0f));
		armour.getSet("Adequate")->add(AI::Fuzzy::Set::Member(0.8f, 1.0f));
		armour.getSet("Adequate")->add(AI::Fuzzy::Set::Member(1.0f, 1.0f));

		AI::Fuzzy::Logic risk("Risk");
		risk.getSet("Low")->add(AI::Fuzzy::Set::Member(0.0f, 1.0f));
		risk.getSet("Low")->add(AI::Fuzzy::Set::Member(0.2f, 1.0f));
		risk.getSet("Low")->add(AI::Fuzzy::Set::Member(0.4f, 0.0f));

		risk.getSet("Med")->add(AI::Fuzzy::Set::Member(0.3f, 0.0f));
		risk.getSet("Med")->add(AI::Fuzzy::Set::Member(0.5f, 1.0f));
		risk.getSet("Med")->add(AI::Fuzzy::Set::Member(0.7f, 0.0f));

		risk.getSet("High")->add(AI::Fuzzy::Set::Member(0.6f, 0.0f));
		risk.getSet("High")->add(AI::Fuzzy::Set::Member(0.8f, 1.0f));
		risk.getSet("High")->add(AI::Fuzzy::Set::Member(1.0f, 1.0f));

		AI::Fuzzy::Set::Ptr a = risk.getSet(0.35f, AI::Fuzzy::Logic::MAX);
		AI::Fuzzy::Set::Ptr b = risk.getSet(0.35f, AI::Fuzzy::Logic::MIN);
		AI::Fuzzy::Set::Ptr c = risk.getSet(0.35f, AI::Fuzzy::Logic::RAND);


		AI::Fuzzy::Set::Member currHealth(0.45f), currArmour(0.65f);
		float badHealth = health.getSet("Bad")->membership(currHealth.value());
		float goodHealth = health.getSet("Good")->membership(currHealth.value());

		float inadequateArmour = armour.getSet("Inadequate")->membership(currArmour.value());
		float marginalArmour = armour.getSet("Marginal")->membership(currArmour.value());
		float adequateArmour = armour.getSet("Adequate")->membership(currArmour.value());

		AI::Fuzzy::EvalRule::Ptr healthGood(new AI::Fuzzy::EvalRule(health.getSet("Good"), currHealth));
		AI::Fuzzy::Rule::Ptr armourAdequate(new AI::Fuzzy::EvalRule(armour.getSet("Adequate"), currArmour));
		AI::Fuzzy::Rule::Ptr healthGoodAndArmourAdequate(new AI::Fuzzy::AndRule(healthGood, armourAdequate));

		AI::Fuzzy::EvalRule::Ptr healthBad(new AI::Fuzzy::EvalRule(health.getSet("Bad"), currHealth));
		AI::Fuzzy::Rule::Ptr armourMarginal(new AI::Fuzzy::EvalRule(armour.getSet("Marginal"), currArmour));
		AI::Fuzzy::Rule::Ptr healthBadAndArmourMarginal(new AI::Fuzzy::OrRule(healthBad, armourMarginal));

		AI::Fuzzy::EvalRule::Ptr armourInadequate(new AI::Fuzzy::EvalRule(armour.getSet("Inadequate"), currArmour));
		AI::Fuzzy::Rule::Ptr healthBadAndArmourInadequate(new AI::Fuzzy::AndRule(healthBad, armourInadequate));

		AI::Fuzzy::RuleSet ruleset; 
		ruleset.add(AI::Fuzzy::RuleSet::Pair(risk.getSet("Low"), healthGoodAndArmourAdequate));
		ruleset.add(AI::Fuzzy::RuleSet::Pair(risk.getSet("Med"), healthBadAndArmourMarginal));
		ruleset.add(AI::Fuzzy::RuleSet::Pair(risk.getSet("High"), healthBadAndArmourInadequate));

		float centroid = ruleset.apply(10);
	}
}
