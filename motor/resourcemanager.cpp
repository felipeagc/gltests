#include "resourcemanager.h"

ResourceManager::ResourceManager()
{

}

void ResourceManager::Remove(const std::string &path)
{
	Resource *r = m_resourceMap[path];
	m_resourceMap.erase(path);
	delete r;
}
