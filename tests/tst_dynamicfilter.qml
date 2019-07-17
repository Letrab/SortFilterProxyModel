import QtQuick 2.0
import SortFilterProxyModel 0.2
import QtQml.Models 2.2
import QtTest 1.1
import QtQml 2.2

Item {

    property var roleValues: [0, 1, 2]
    property var roleValuesNew: [3]

    Instantiator {
        id: instantiator
        model: roleValues
        ValueFilter {
            roleName: "a"
            FilterContainer.container: testContainer
            value: modelData
        }
    }

    ListModel {
        id: dataModel
        ListElement { a: 0; b: true}
        ListElement { a: 1; b: false}
        ListElement { a: 2; b: true}
        ListElement { a: 3; b: false}
    }

    SortFilterProxyModel {
        id: testModel
        sourceModel: dataModel
        filters: AnyOf {
            id: testContainer
        }
    }

    TestCase {
        name:"DynamicFilterTest"

        function modelValues() {
            var modelValues = [];

            for (var i = 0; i < testModel.count; i++)
                modelValues.push(testModel.get(i));

            return modelValues;
        }

        function test_filterContainers(filter) {
            compare(JSON.stringify(modelValues()), JSON.stringify([{"a":0,"b":true},{"a":1,"b":false},{"a":2,"b":true}]));
            instantiator.model = roleValuesNew
            wait(100) // Wait for destruction of Instantiator items
            compare(JSON.stringify(modelValues()), JSON.stringify([{"a":3,"b":false}]));
        }
    }
}
