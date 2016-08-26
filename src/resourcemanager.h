#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <map>
#include <memory>

#include "resource.h"

class ResourceManager
{
public:
	ResourceManager();
	template <class T> T *Get(const std::string &path) {
		auto found = m_resourceMap.find(path);
		if(found != m_resourceMap.end())
		{
			// Found the resource
			return dynamic_cast<T*>(found->second);
		}
		else
		{
			// Didn't find it, create it
			m_resourceMap.insert(std::pair<std::string, Resource*>(path, new T{path}));
			return Get<T>(path);
		}
	}

	void Remove(const std::string &path);

	std::map<std::string, Resource*> m_resourceMap;
};

#endif // RESOURCEMANAGER_H
