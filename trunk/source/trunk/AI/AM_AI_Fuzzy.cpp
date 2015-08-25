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

#include <vector>
#include "AM_Math.hpp"
#include "AM_Random.hpp"

#include "AM_AI_Fuzzy.hpp"

namespace AutoMatic
{
	namespace AI
	{
		namespace Fuzzy
		{
			//****************************************************************************//
			Set Set::Member::clamp(Member const & clamp_, Member const & left_, Member const & right_) const
			{
				float val;
				Set out;
				if(membership() <= clamp_.membership() ){ out.add(*this); }
				else
				{
					val = (!left_.isSet() ? value() : value() + ((value() - value()) * clamp_.membership()) / membership() );
					out.add( Member(val, clamp_.membership()) );
				}

				if(right_.isSet())
				{
					float biggerMembership = (right_.membership() > membership() ? right_.membership() : membership());
					float clampMembership = (right_.membership() > membership() ? clamp_.membership() : 1 - clamp_.membership());

					val = value() + ((right_.value() - value()) * clampMembership) / biggerMembership;
					out.add( Member(val, clamp_.membership()) );
				}

				return out;
			}

			//****************************************************************************//
			Set const & Set::operator+=(Set const & fuzzySet_)
			{
				if(&fuzzySet_ == this){ return *this; }
				for(std::set<Member>::const_iterator i = fuzzySet_._members.begin(); 
					i != fuzzySet_._members.end(); ++i)
				{
					_members.insert( *i );
				}

				return *this;
			}

			float Set::membership(float value_) const
			{
				Member smaller, larger;
				// For each fuzzy member in the set...
				for(std::set<Member>::const_iterator i = _members.begin(); i != _members.end(); ++i)
				{
					// If the desired value is in the set, return that membership.
					if( (*i) == value_ ){ return (*i).membership(); }
					// If the member is smaller than the value, 
					else if( (*i) < value_ )
					{ // Set the lower bound
						smaller = (*i);
					}
					// If the member is greater than the value,
					else if( (*i) > value_ )
					{ 
						// Otherwise set the upper bound and leave (the set is sorted ascending).
						larger = (*i); break;
					}
				}

				// There was no member smaller than the value, then return the largest one found.
				if (!smaller.isSet()){ return	larger.membership(); }
				// There was no member larger than the value, then return the largest one found.
				if(!larger.isSet()){ return smaller.membership(); }

				float percent = (value_ - smaller.value()) / (larger.value() - smaller.value());

				return smaller.membership() + (larger.membership() - smaller.membership())*percent;
			}

			Set Set::setUnion(Set const & fuzzySet_) const
			{
				Set out;
				float temp = 0.0f;

				for(std::set<Member>::const_iterator i = _members.begin(); i != _members.end(); ++i)
				{
					temp = fuzzySet_.membership((*i).value());
					out.add( Member((*i).value(), (temp > (*i).membership() ? temp : (*i).membership())) );
				}

				for(std::set<Member>::const_iterator i = fuzzySet_._members.begin(); i != fuzzySet_._members.end(); ++i)
				{
					temp = membership((*i).value());
					out.add( Member((*i).value(), (temp > (*i).membership() ? temp : (*i).membership())) );
				}

				out += intersectionPoints(fuzzySet_);
				out += fuzzySet_.intersectionPoints(*this);

				return out;
			}

			Set Set::intersectionPoints(Set const & fuzzySet_) const
			{
				Set out;

				std::set<Member>::const_iterator i = _members.begin(), j;
				std::set<Member>::const_iterator k = fuzzySet_._members.begin(), l;

				if(k == fuzzySet_._members.end() || i == _members.end()){ return out; }

				while(k != fuzzySet_._members.end())
				{ // Find first member of fuzzySet_ that is greater than the first member this set.
					if ( (*k).value() > (*i).value() ){	break; }
					++k;
				}

				if(k == fuzzySet_._members.end()){ return out; }

				float m, n, c, d, val, mem, mem2;

				Vector v;
				while (i != _members.end())
				{ // For every member...
					j = i; ++i; if(i == _members.end()){ break; }
					if((*i).value() < (*k).value()){ continue; } // Continue until j is the point closest to k on the left.
					l = k; ++k; if(k == fuzzySet_._members.end()){ break; }

					// Find intersection, if available...
					v = Intersect( (*j).value(), (*j).membership(), (*i).value(), (*i).membership(), 
						(*l).value(), (*l).membership(), (*k).value(), (*k).membership() );

					out.add(Member(v.x(), v.y()));
				}

				return out;
			}

			Set Set::clamp(Member const & clamp_) const
			{
				Set out;
				Member smaller, larger;
				float val;
				for(std::set<Member>::const_iterator i = _members.begin(); i != _members.end(); ++i)
				{ 
					std::set<Member>::const_iterator j = i; j++;
					if(j != _members.end()){ larger = *j; }else{ larger.isSet(false); }
					out += (*i).clamp(clamp_, smaller, larger);
					smaller = *i;
				}

				return out;
			}

			float Slope( Set::Member const & a_, Set::Member const & b_)
			{
				float t = (b_.value() - a_.value()); if(t == 0.0f){ return 0.0f; }
				return (b_.membership() - a_.membership()) / t; 
			}

			Set Set::minimise(float tolerance_) const
			{
				Set out;
				float slope1, slope2;
				std::set<Member>::const_iterator i=_members.begin(),j,k;

				out.add(*i);

				while (i != _members.end())
				{
					j = i; ++i;
					if(i == _members.end()){ break; }
					k = i; ++i;
					if(i == _members.end()){ break; }

					do 
					{
						slope1 = Slope(*j, *k);
						slope2 = Slope(*j, *i);

						if(slope1 <= slope2 + tolerance_ && slope1 >= slope2 - tolerance_)
						{	slope1 = slope2; }

						if (slope1 == slope2)
						{
							k = i; ++i;
							if(i == _members.end()){ out.add(*k); break; }
						}
						else
						{
							out.add(*k);
							i = k;
						}
					} while(slope1 == slope2);
				}
				return out;
			}

			float Set::centroid(int slices_) const
			{
				Member out;
				float min = 0, max = (*_members.rbegin()).value();
				float dx = (max - min) / static_cast<float>(slices_);

				float memSum = 0.0f, memValSum = 0.0f;
				float mem = 0.0f; int slice = 0;
				while(slice < slices_ && min <= max)
				{
					mem = membership(min);
					if(mem > 0.0f)
					{
						memSum += mem;
						memValSum += mem*min;
						slice++;
					}

					min += dx; 
				}

				return memValSum / memSum;
			}
			//****************************************************************************//
			Set::Ptr Logic::getSet(float value_, Search search_)
			{
				Set::Ptr ret;
				std::vector<float> weightedMemberships;
				float weightedMembershipTotal = 0.0f;

				bool isSet = false;
				float membership = 0.0f, temp = 0.0f;

				std::map<Value::ID, Value::Ptr>::iterator search = _values.begin();
				for (std::map<Value::ID, Value::Ptr>::iterator i = _values.begin(); i != _values.end(); ++i)
				{
					Fuzzy::Set * setPtr = static_cast<Fuzzy::Set *>( (*i).second.get() );
					membership = setPtr->membership(value_)*setPtr->influence();
					if( membership != 0.0f && (!isSet || 
						((search_ == MAX && membership > temp) || 
						(search_ == MIN && membership < temp))) )
					{
						temp = membership; search = i; isSet = true;
					}
					weightedMemberships.push_back( membership);
					weightedMembershipTotal += membership;
				}

				if(weightedMembershipTotal != 0.0f)
				{
					if(search_ == RAND)
					{
						float	rand = Random::Get()->randomF(0.0f, weightedMembershipTotal);
						weightedMembershipTotal = 0.0f;
						for(std::vector<float>::iterator i = weightedMemberships.begin(); 
							i != weightedMemberships.end(); ++i, ++search)
						{
							if( (*i) <= 0.0f ){ continue; }
							weightedMembershipTotal += (*i);
							if( weightedMembershipTotal >= rand ){	break; }
						}
					}
					if(search != _values.end()){ ret = boost::static_pointer_cast<Fuzzy::Set>((*search).second); }
				}

				return ret;
			}
			//****************************************************************************//
			float RuleSet::apply(int slices_)
			{
				Fuzzy::Set centroidSet;
				Fuzzy::Set::Member member;

				for (std::set< std::pair<Set::Ptr, Rule::Ptr> >::iterator i = _rules.begin();
					i != _rules.end(); ++i)
				{
					centroidSet = centroidSet.setUnion((*i).first->clamp((*i).second->apply())); 
				}

				return centroidSet.centroid(slices_);
			}
		}
	}
}
