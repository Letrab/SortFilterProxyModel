#include "filtercontainerfilter.h"

namespace qqsfpm {

FilterContainerAttached::FilterContainerAttached(QObject *parent)
    : QObject (parent), m_parentFilter(qobject_cast<Filter*>(parent))
{} // Cash parent filter pointer, because this object can be deleted before the Filter itself.

FilterContainerAttached::~FilterContainerAttached()
{
    if (m_parentFilter && m_container)
    {
        m_container->removeFilter(m_parentFilter); // Remove filter from container
    }
}

/*!
    \qmlattachedproperty var FilterContainer::container()

    This property attaches a container to a \l Filter.
*/
FilterContainerFilter* FilterContainerAttached::container() const
{
    return m_container;
}

void FilterContainerAttached::setContainer(FilterContainerFilter* container)
{
    if (m_container == container)
        return;

    auto filter = qobject_cast<Filter*>(parent());
    if (filter)
    {
        if(m_container)
        {
            m_container->removeFilter(filter); // Remove filter from old container
        }
        container->appendFilter(filter); // Add filter to new container
    }

    m_container = container;
    Q_EMIT containerChanged();
}

void FilterContainerFilter::proxyModelCompleted(const QQmlSortFilterProxyModel& proxyModel)
{
    for (Filter* filter : m_filters)
        filter->proxyModelCompleted(proxyModel);
}

FilterContainerAttached* FilterContainerFilter::qmlAttachedProperties(QObject *object)
{
    return new FilterContainerAttached(object);
}

void FilterContainerFilter::onFilterAppended(Filter* filter)
{
    connect(filter, &Filter::invalidated, this, &FilterContainerFilter::invalidate);
    invalidate();
}

void FilterContainerFilter::onFilterRemoved(Filter* filter)
{
    Q_UNUSED(filter)
    invalidate();
}

void qqsfpm::FilterContainerFilter::onFiltersCleared()
{
    invalidate();
}

}
