#include "ee2/CombineAO.h"

namespace ee2
{

void CombineAO::Undo()
{
	for (int i = m_atomics.size() - 1; i >= 0; --i) {
		m_atomics[i]->Undo();
	}
}

void CombineAO::Redo()
{
	for (int i = 0, n = m_atomics.size(); i < n; ++i) {
		m_atomics[i]->Redo();
	}
}

void CombineAO::Add(const std::shared_ptr<ee0::AtomicOP>& ao)
{
	m_atomics.push_back(ao);
}

}