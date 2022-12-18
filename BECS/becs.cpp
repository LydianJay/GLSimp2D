#include "becs.h"

ComponentID BECS::registerComponent(unsigned int size) {

	m_registeredComp.push_back(m_compIDCounter);
	CompMemCell cell;
	cell.m_compSize = size;
	m_compMemory.push_back(std::move(cell));
	m_freeCompMemory.push_back(std::move(std::queue<FreeMem>()));
	ComponentID id = m_compIDCounter;
	m_compIDCounter++;
	return id;
}

ComponentID* BECS::getComponentBinded(EntityID entID, unsigned int * size)
{
	if (!(entID < m_entIDCounter))return nullptr;
	
	// check if this entity is deleted
	auto isValid = m_entity.find(entID);
	if (isValid == m_entity.end())return nullptr;

	*size = m_entity[entID].m_bindedComp.size();
	
	return m_entity[entID].m_bindedComp.data();
}


void BECS::addComponent(EntityID entID, ComponentID compID) {
	if (!(entID < m_entIDCounter))return;
	if (!(compID < m_compIDCounter))return;
	
	
	// check if this entity is deleted
	auto isValid = m_entity.find(entID);
	if (isValid == m_entity.end())return;
	
	// check if the componennt is already bound to the entity
	for (ComponentID& c : m_entity[entID].m_bindedComp) {
		if (c == compID)return;
	}

	if (!m_freeCompMemory[compID].empty()) {
		CompMemIndex temp;
		m_entity[entID].m_bindedComp.push_back(compID);
		FreeMem mem = m_freeCompMemory[compID].front();
		m_freeCompMemory[compID].pop();
		temp.id = mem.id;
		temp.index = mem.compMemIndex;
		
		m_entity[entID].m_memIndex.insert(std::make_pair(compID, temp));
		m_compMemory[compID].m_components[mem.compMemIndex].isValid = true;
		m_compMemory[compID].m_components[mem.compMemIndex].m_entOwner = entID;
		return;
	}
	
	MemCell cell;
	cell.m_comp = new char[m_compMemory[compID].m_compSize]; 
	cell.m_entOwner = entID;
	cell.isValid = true;

	m_entity[entID].m_bindedComp.push_back(compID);
	m_compMemory[compID].m_components.push_back(std::move(cell));
	CompMemIndex index;

	index.index = m_compMemory[compID].m_components.size() - 1;
	index.id = compID;
	m_entity[entID].m_memIndex.insert(std::make_pair(compID, index));

}

void BECS::removeComponent(EntityID entID, ComponentID compID){

	if (!(entID < m_entIDCounter))return;
	if (!(compID < m_compIDCounter))return;
	// need to check if in free space
	if (m_entity[entID].m_bindedComp.empty())return;
	auto isValid = m_entity.find(entID);
	if (isValid == m_entity.end())return;

	int index = 0;
	bool isCompontBound = false;
	for (ComponentID& c : m_entity[entID].m_bindedComp) {
		
		if (c == compID) {
			isCompontBound = true;
			break;
		}
		index++;
	}

	if (!isCompontBound)return;
	
	
	

	FreeMem mem;
	mem.id = compID;
	
	
	
	// compMemIndex is the index for the component memory

	mem.compMemIndex = m_entity[entID].m_memIndex[compID].index;

	m_compMemory[compID].m_components[mem.compMemIndex].isValid = false;
	m_compMemory[compID].m_components[mem.compMemIndex].m_entOwner = 0xFFFFFFFF;
	m_entity[entID].m_memIndex.erase(compID);
	m_freeCompMemory[compID].push(std::move(mem));
	m_entity[entID].m_bindedComp.erase(m_entity[entID].m_bindedComp.begin() + index);

}

void BECS::destroyEntity(EntityID entID){
	if (!(entID < m_entIDCounter))return;
	
	// check if this entity is deleted
	auto isValid = m_entity.find(entID);

	if (isValid == m_entity.end())return;


	


	std::vector<ComponentID> temp = m_entity[entID].m_bindedComp;
	auto& entRef = temp;
	for (auto comp : entRef) {
		removeComponent(entID, comp);

	}
	


	m_entity[entID].m_bindedComp = std::move(std::vector<ComponentID>());
	m_entity[entID].m_memIndex = std::move(std::unordered_map<ComponentID, CompMemIndex>());
	m_entity.erase(entID);
	m_freeEntity.push(entID);
}



EntityID BECS::createEntity()
{

	if (!m_freeEntity.empty()) {

		EntityID id = m_freeEntity.front();
		Entity ent;
		ent.m_ID = id;
		
		m_freeEntity.pop();
		m_entity.insert(std::move(std::make_pair(id, std::move(ent))));
		return id;
	}

	EntityID id = m_entIDCounter;
	m_entIDCounter++;
	Entity ent;
	ent.m_ID = id;
	m_entity.insert(std::move(std::make_pair(id, std::move(ent))));

	return id;
}

QueryComponent BECS::getComponent(ComponentID compID)
{
	QueryComponent component;

	component.m_compArry = m_compMemory[compID].m_components.data();
	component.m_arryCount = m_compMemory[compID].m_components.size();
	component.m_compSize = m_compMemory[compID].m_compSize;
	component.m_ID = compID;
	return std::move(component);
}

void* BECS::getEntityComponent(EntityID entID, ComponentID compID)
{	
	if (!(entID < m_entIDCounter))return nullptr;
	if (!(compID < m_compIDCounter))return nullptr;
	// need to check if in free space

	auto isValid = m_entity.find(entID);
	if (isValid == m_entity.end())return nullptr;



	int index = m_entity[entID].m_memIndex[compID].index;
	return m_compMemory[compID].m_components[index].m_comp;
}

void BECS::debug() {
	std::cout << "---------------------------------\n";
	std::cout << "Entity Count: " << m_entity.size() << '\n';
	std::cout << "---------------------------------\n";
	
	
	for (const std::pair<EntityID,Entity>& c : m_entity) {
		std::cout << "Entity ID: " << c.first << '\n';
		for (const ComponentID& cID : c.second.m_bindedComp) {
			std::cout << "Comp ID: " << cID << " Index: " << c.second.m_memIndex.at(cID).index << '\n';
		}
		std::cout << "\n---------------------------------\n";
	}
	
	
	std::cout << "---------------------------------\n";
	std::cout << "Component Memory Cell Count: " << m_compMemory.size() << '\n';
	std::cout << "---------------------------------\n";

	for (const CompMemCell& c : m_compMemory) {

		std::cout << "Size: " << c.m_compSize << '\t' ;
		uint32_t index = 0;
		for (const MemCell& a : c.m_components) {
			std::cout << "\nEntity Owner: " << a.m_entOwner << " Index: " << index << " Raw Location: " << a.m_comp << '\n';
			index++;
		}

	}

	std::cout << "\n";
	
	std::cout << "---------------------------------\n";
	std::cout << "Free Memory: " << m_freeCompMemory.size() << '\n';
	std::cout << "---------------------------------\n";
	size_t n = m_freeCompMemory.size();
	for (size_t i = 0; i < n; i++) {

		std::cout << "Comp ID: " << i << '\n';
		const auto& ref = m_freeCompMemory[i]._Get_container();
		
		for (const FreeMem& f : ref) {

			std::cout << "Comp Index: " << f.compMemIndex << '\n';

		}


	}



}


BECS::~BECS() {

	for (CompMemCell& cell : m_compMemory) {
		for (MemCell& memcell : cell.m_components) {
			delete[] memcell.m_comp;
		}
		cell.m_components.clear();
	}
	m_registeredComp.clear();
	m_compMemory.clear();
	m_freeCompMemory.clear();
	m_entity.clear();
}
