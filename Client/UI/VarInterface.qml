import QtQuick
import QtQuick.Controls

Item {
	// Property getters
	property var getName: function() {
		console.log("ParamInItem: getName() default handler");
		return "<default>";
	}

	property var getValue: function() {
		console.log("ParamInItem: getValue() default handler");
		return "<default>";
	};

	property var getValueStr: function() {
		console.log("ParamInItem: getValueStr() default handler");
		return "<default>";
	}

	property var getType: function() {
		console.log("ParamInItem: getType() default handler");
		return "<default>";
	}

	property var getTypeStr: function() {
		console.log("ParamInItem: getTypeStr() default handler");
		return "<default>";
	}

	property var getProtoName: function() {
		console.log("ParamInItem: getProtoName() default handler");
		return "<default>";
	}

	property var getParentModel: function() {
		console.log("ParamInItem: getParentModel() default handler");
		return "<default>";
	}

	// Property setters
	property var setName: function(val) {
		console.log("ParamInItem: setName(" + val + ") default handler");
	}

	property var setValue: function(val) {
		console.log("ParamInItem: setValue(" + val + ") default handler");
	}

	property var setProtoName: function(val) {
		console.log("ParamInItem: setValueStr(" + val + ") default handler");
	}

	property var setType: function(val) {
		console.log("ParamInItem: setType(" + val + ") default handler");
	}

	property var setParentModel: function(val) {
		console.log("ParamInItem: setParentModel(" + val + ") default handler");
	}
}
