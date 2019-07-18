#ifndef FILTERCONTAINERFILTER_H
#define FILTERCONTAINERFILTER_H

#include "filter.h"
#include "filtercontainer.h"
#include <QtQml>
#include <QPointer>

namespace qqsfpm {

class FilterContainerFilter;

class FilterContainerAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FilterContainerFilter* container READ container WRITE setContainer NOTIFY containerChanged)
public:
    FilterContainerAttached(QObject *parent);
    ~FilterContainerAttached();

    FilterContainerFilter* container() const;
    void setContainer(FilterContainerFilter* container);

Q_SIGNALS:
    void containerChanged();

private:
    QPointer<FilterContainerFilter> m_container;
    Filter* m_parentFilter;
};

class FilterContainerFilter : public Filter, public FilterContainer {
    Q_OBJECT
    Q_INTERFACES(qqsfpm::FilterContainer)
    Q_PROPERTY(QQmlListProperty<qqsfpm::Filter> filters READ filtersListProperty NOTIFY filtersChanged)
    Q_CLASSINFO("DefaultProperty", "filters")

public:
    using Filter::Filter;

    void proxyModelCompleted(const QQmlSortFilterProxyModel& proxyModel) override;

    static FilterContainerAttached* qmlAttachedProperties(QObject *object);

Q_SIGNALS:
    void filtersChanged();

private:
    void onFilterAppended(Filter* filter) override;
    void onFilterRemoved(Filter* filter) override;
    void onFiltersCleared() override;
};

}

QML_DECLARE_TYPEINFO(qqsfpm::FilterContainerFilter, QML_HAS_ATTACHED_PROPERTIES)

#endif // FILTERCONTAINERFILTER_H
