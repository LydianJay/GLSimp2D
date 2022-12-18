#pragma once
#include "types.h"
#include <queue>

/*
*	 
*	MIT License
*	
*	Copyright (c) 2022 Lloyd Jay Arpilleda Edradan
*	
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*	
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*	
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*	 
*/

#include <iostream>

class BECS
{
public:
	~BECS();
	ComponentID				registerComponent(unsigned int size);
	ComponentID*			getComponentBinded(EntityID entID, unsigned int* size);
	void					addComponent(EntityID entID, ComponentID compID);
	void					removeComponent(EntityID entID, ComponentID compID);
	void					destroyEntity(EntityID id);
	EntityID				createEntity();
	QueryComponent			getComponent(ComponentID);
	void*					getEntityComponent(EntityID, ComponentID);
	


	void					debug();

private:
	EntityID								m_entIDCounter;
	ComponentID								m_compIDCounter;
	std::vector<ComponentID>				m_registeredComp;
	std::vector<CompMemCell>				m_compMemory;
	std::vector<std::queue<FreeMem>>		m_freeCompMemory;
	std::unordered_map<EntityID, Entity>	m_entity;
	std::queue<EntityID>					m_freeEntity;
};

