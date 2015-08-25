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

#ifndef _AM_AI_Fuzzy_hpp_
#define _AM_AI_Fuzzy_hpp_

#include <set>

#include "AM_AI_Part.hpp"
#include "AM_AI_Value.hpp"

namespace AutoMatic
{
	namespace AI
	{
		/// The Fuzzy namespace contains all the classes that work on Fuzzy Logic.
		namespace Fuzzy
		{
			/// A Fuzzy Set contains a set of Members (value/membership pairs).
			/** This set defines the domain of the set.
			*/
			class Set : public Value
			{
			public:
				typedef boost::shared_ptr<Set> Ptr;

				class Member
				{
				protected:
					float _value, _membership;
					bool _isSet;

				public:
					Member(void) : _isSet(false) {}

					Member(float value_, float membership_ = 0.0f)
						: _value(value_), _membership(membership_), _isSet(true) 
					{
						if(_membership > 1.0f){ _membership = 1.0f; }
						if(_membership < 0.0f){ _membership = 0.0f; }
					}

					Member(Member const & member_)
						: _value(member_._value), _membership(member_._membership), _isSet(member_._isSet) {}

					Member const & operator=(Member const & member_)
					{
						_value = member_._value; _membership = member_._membership; _isSet = member_._isSet;
						return *this;
					}

					void set(float value_, float membership_)
					{
						_value = value_; _membership = membership_; _isSet = true;
						if(_membership > 1.0f){ _membership = 1.0f; }
						if(_membership < 0.0f){ _membership = 0.0f; }
					}

					bool operator==(Member const & member_) const {	return _value == member_._value;	}
					bool operator<(Member const & member_) const { return _value < member_._value; }
					bool operator>(Member const & member_) const { return _value > member_._value; }

					void isSet(bool isSet_){ _isSet = isSet_; }
					bool isSet(void) const { return _isSet; }
					float value(void) const { return _value; }
					float membership(void) const { return _membership; }

					/// Get a set of points, clamped to \a clamp_, for this member, if this member is > \a clamp_.
					/** Uses \a left_ and \a right_ to determine where points need to be added to clamp this point.
					*	\return Returns a set of points that result from clamping the member.
					*/
					Set clamp(Member const & clamp_, Member const & left_, Member const & right_) const;
				};


				Set(ID const & id_) : Value(id_), _influence(1.0f) {}
				Set(Set const & fuzzySet_)
					: Value(fuzzySet_._id), _members(fuzzySet_._members), _influence(fuzzySet_._influence)
				{}

				Set const & operator+=(Set const & fuzzySet_);

				/// Gets the membership of a value in the set.
				/** \todo Will this work for negative values in the set?
				*/
				float membership(float value_) const;

				/// Returns the union of this set and \a fuzzySet_
				/**
				*/
				Set setUnion(Set const & fuzzySet_) const;

				/// Returns the points where the sets intersect.
				Set intersectionPoints(Set const & fuzzySet_) const;

				/// Clamp all members greater in membership than \a clamp_ to \a clamp_.
				/** The size of the returned set may be greater than the original set.
				* The returned set is not minimised automatically.
				*/
				Set clamp(Member const & clamp_) const;

				/// Remove all redundant members from the set.
				/** \return Returns a set with all redundant members removed.
				* Redundant members are those that lie on the line between its neighbouring
				* members.
				*/
				Set minimise(float tolerance_ = 0.00001) const;

				/// Find the centre of gravity of the set.
				float Set::centroid(int slices_ = 10) const;

				void add( Member const & member_ ){ _members.insert(member_); }
				void rem( Member const & member_ ){ _members.erase(member_); }

				/// Influence is a multiplier used in Logic::getSet(float value_, Search search_).
				float influence(void){ return _influence; }
				void influence(float influence_){ _influence = influence_; }

			protected:
				friend class RuleSet;

				Set(void) : Value("") {}
				std::set< Member > _members;
				float _influence;
			};
			//****************************************************************************//
			class Logic : public Part
			{
			public:
				typedef std::string ID;
				typedef boost::shared_ptr<Logic> Ptr;
				enum Search { MIN, MAX, RAND };

				Logic(ID const & id_) : Part(id_) {}

				/// Get a set for the value.
				/** 1. For each set, get the membership for \a value_.
				* - For Rand -
				* 2. Multiply the membership values by the set influences.
				* 3. Add membership values together.
				* 4. Generate a random number.
				* 5. Sum values until the sum >= random number.  
				* 6. Return the set that returned the member that exceeded the random number.
				* - For Max -
				* 2. Return the set the \a value_ has the greatest membership in.
				* - For Min -
				* 2. Return the set the \a value_ has the minimum membership in.
				* \todo Generate random number.
				*/
				Set::Ptr getSet(float value_, Search search_);

				Set::Ptr getSet(Set::ID const & id_)
				{
					if(!_values[id_]){ _values[id_].reset(new Set(id_)); }
					return boost::static_pointer_cast<Set>( _values[id_] );
				}
			protected:
				Value * generateValue(Set::ID const & id_){ return new Set(id_); }
			};
			//****************************************************************************//
			/// Rule class for developing fuzzy rules.
			/**
			*/
			class Rule
			{
			public:
				typedef boost::shared_ptr<Rule> Ptr;

				Rule(void) {}

				virtual Set::Member apply(void) = 0;
			};

			class AndRule : public Rule
			{
			public:
				AndRule(Rule::Ptr left_, Rule::Ptr right_)
					: _left(left_), _right(right_) {}

				virtual Set::Member apply(void)
				{
					Set::Member left = _left->apply(), right = _right->apply();
					return (left.membership() < right.membership() ? left : right);
				}

			protected:
				Rule::Ptr _left, _right;
			};

			class OrRule : public Rule
			{
			public:

				OrRule(Rule::Ptr left_, Rule::Ptr right_)
					: _left(left_), _right(right_) {}

				virtual Set::Member apply(void)
				{
					Set::Member left = _left->apply(), right = _right->apply();
					return (left.membership() > right.membership() ? left : right);
				}

			protected:
				Rule::Ptr _left, _right;
			};

			class EvalRule : public Rule
			{
			public:
				typedef boost::shared_ptr<EvalRule> Ptr;

				EvalRule(Set::Ptr set_, Set::Member member_)
					: _set(set_), _member(member_) {}

				void value(float member_){ _member = member_; }

				virtual Set::Member apply(void)
				{
					return Set::Member(_member.value(), _set->membership(_member.value()));
				}

			protected:
				Set::Ptr _set;
				Set::Member _member;
			};

			class RuleSet
			{
			public:
				typedef std::pair<Set::Ptr, Rule::Ptr> Pair;

				void add(Pair pair_){ _rules.insert(pair_); }
				void rem(Pair pair_){ _rules.erase(pair_); }

				float apply(int slices_ = 10);

			protected:
				std::set< std::pair<Set::Ptr, Rule::Ptr> > _rules;
			};
		}
	}
}

#endif //_AM_AI_Fuzzy_hpp_
