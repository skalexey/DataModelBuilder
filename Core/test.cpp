#include <iostream>
#include "CoreConfig.h"
#include "DMBCore.h"

void Load(const std::string& fName)
{
	std::cout << "Load '" << fName << "'\n";
	dmb::Model model;
	model.Load(fName);
	std::cout << "Model loaded from the file'" << fName << "':\n";
	std::cout << model.JSONStr({ true }) << "\n";
}

void ModelCreationTest()
{
	std::cout << "Model creation test\n";
	dmb::Model model;
	auto& registry = model.GetRegistry();
	auto& privateScope = model.GetPrivateScope();
	auto& pear = registry.CreateType("pear");
	pear.Set("isFruit", true);
	pear.Set("color", "Yellow");
	pear.Set("radius", 0.3f);
	pear.Set("branchCount", 1);
	auto bush = vl::Object();
	bush.Set("leafColor", "Green");
	bush.Set("isTree", true);
	bush.Set("leafPerBranch", 6);
	bush.Set("branches", vl::List());
	bush.Set("x", 0);
	bush.Set("y", 0);
	registry.RegisterType("bush", bush);
	auto branch = vl::Object();
	privateScope.RegisterType("branch", branch);
	branch.Set("leafCount", 10);
	branch.Set("fruit", vl::Object());
	branch.Set("branches", vl::List());
	auto branch1 = branch.Copy();
	branch1.Set("leafCount", 9);
	auto branch2 = branch.Copy();
	branch2.Set("leafCount", 3);
	branch2.Set("fruit", pear);
	branch1.Get("branches").AsList().Add(branch2);
	bush.Get("branches").AsList().Add(branch1);
	std::cout << "Model created\n";
	const char* fName = "new_model.json";
	model.Store(fName, { true, true });
	std::cout << "Model written to the json file'" << fName << "':\n";
	std::cout << model.JSONStr({ true }) << "\n";
}

void CrashTest()
{
	std::cout << "Crash test\n";
	dmb::Model model;
	auto& registry = model.GetRegistry();
	auto& privateScope = model.GetPrivateScope();
	auto& pear = registry.CreateType("pear");
	pear.Set("isFruit", true);
	pear.Set("color", "Yellow");
	pear.Set("radius", 0.3f);
	pear.Set("branchCount", 1);
	auto bush = vl::Object();
	bush.Set("leafColor", "Green");
	bush.Set("isTree", true);
	bush.Set("leafPerBranch", 6);
	bush.Set("branches", vl::List());
	bush.Set("x", 0);
	bush.Set("y", 0);
	registry.RegisterType("bush", bush);
	auto branch = vl::Object();
	privateScope.RegisterType("branch", branch);
	branch.Set("leafCount", 10);
	branch.Set("fruit", vl::Object());
	branch.Set("branches", vl::List());
	auto branch1 = branch.Copy();
	branch1.Set("leafCount", 9);
	auto branch2 = branch.Copy();
	branch2.Set("leafCount", 3);
	branch2.Set("fruit", pear);
	branch1.Get("branches").AsList().Add(branch2);
	bush.Get("branches").AsList().Add(branch1);
	std::cout << "Model created\n";
	const char* fName = "new_model.json";
	model.Store(fName, { true, true });
	std::cout << "Model written to the json file'" << fName << "':\n";
	std::cout << model.JSONStr({ true }) << "\n";
}

void ModelEditTest()
{
	std::cout << "Model edit test\n";
	Load("content_filling_test_out.json");
}

void ContentFillingTest()
{
	std::cout << "Content filling test\n";
	dmb::Model model;
	model.Load("new_model.json");
	auto& content = model.GetContent();
	auto& tree = content.Add("fruitTree", model.GetType("bush"));
	std::cout << "Object 'bush' instantiated into the content with name 'fruitTree'\n";
	tree.Set("leafColor", "Gold");
	std::cout << "leafPerBranch: " << tree.Get("leafPerBranch").AsNumber().Val() << "\n";
	content.Add("fieldSize", vl::NumberVar(4));
	auto& animals = content.Add("animalsAround", vl::List()).AsList();
	auto& registry = model.GetRegistry();
	auto& privateScope = model.GetPrivateScope();
	auto& animal = registry.CreateType("animal");
	animal.Set("type", "");
	vl::Object bear;
	bear.SetPrototype(animal);
	bear.Set("type", "bear");
	bear.Set("isMammal", true);
	vl::Object bird;
	bird.SetPrototype(animal);
	bird.Set("type", "bird");
	animals.Add(bear);
	animals.Add(bird);
	animals.Add(vl::Object());
	const char* fName = "content_filling_test_out.json";
	model.Store(fName, { true });
	std::cout << "Written to '" << fName << "': \n" << model.JSONStr({ true }) << "\n";
}

void SeparateContentStoreTest()
{
	std::cout << "Separate content store test\n";
	const char* fName = "content_filling_test_out.json";
	dmb::Model model;
	if (model.Load(fName))
		std::cout << "Model loaded from '" << fName << "'\n";
	else
	{
		std::cout << "Error while loading a model from '" << fName << "'\n";
		return;
	}
	const char* outFName = "separate_content.json";
	if (model.GetContent().Store(outFName, { true }))
		std::cout << "Content stored into the file '" << outFName << "':\n"
			<< model.GetContent().JSONStr({ true }) << "\n";
	else
		std::cout << "Error while storing the content\n";
}

void RenameTest()
{
	std::cout << "Rename test\n";
	dmb::Model model;
	const char* fName = "new_model.json";
	model.Load(fName);
	auto& registry = model.GetRegistry();
	if (registry.HasType("pear"))
	{
		if (registry.GetType("pear").RenameProperty("radius", "diameter"))
			std::cout << "Pear's 'radius' property renamed to 'diameter'\n";
		else
			std::cout << "Can't rename property 'radius'\n";
	}
	else
	{
		std::cout << "Can't find 'pear' type in the model loaded from '" << fName << "'\n";
	}
}

void CustomProtoTestStore()
{
	std::cout << "Custom proto store test\n";
	dmb::Model model;
	auto& registry = model.GetRegistry();
	auto& privateScope = model.GetPrivateScope();
	auto pear = vl::Object();
	pear.Set("isFruit", true);
	pear.Set("color", "Yellow");
	pear.Set("radius", 0.3f);
	pear.Set("branchCount", 1);
	auto apple = vl::Object();
	apple.Set("isFruit", true);
	apple.Set("color", "Green");
	apple.Set("radius", 0.5f);
	apple.Set("branchCount", 1);
	auto orange = vl::Object();
	orange.Set("isFruit", true);
	orange.Set("color", "Orange");
	orange.Set("radius", 0.7f);
	orange.Set("branchCount", 1);
	auto& content = model.GetContent();
	auto fruitList = vl::List();
	fruitList.Add(pear);
	fruitList.Add(apple);
	fruitList.Add(orange);
	content.GetData().Set("fruitList", fruitList);
	auto bush = vl::Object();
	bush.Set("leafColor", "Green");
	bush.Set("isTree", true);
	bush.Set("leafPerBranch", 6);
	bush.Set("branches", vl::List());
	bush.Set("x", 0);
	bush.Set("y", 0);
	registry.RegisterType("bush", bush);
	auto branch = vl::Object();
	privateScope.RegisterType("branch", branch);
	branch.Set("leafCount", 10);
	branch.Set("fruit", vl::Object());
	branch.Set("branches", vl::List());
	auto branch1 = branch.Copy();
	branch1.Set("leafCount", 9);
	auto branch2 = branch.Copy();
	branch2.Set("leafCount", 3);
	auto fruit = vl::Object();
	fruit.SetPrototype(orange);
	branch2.Set("fruit", fruit);
	branch1.Get("branches").AsList().Add(branch2);
	bush.Get("branches").AsList().Add(branch1);
	std::cout << "Model created\n";
	const char* fName = "custom_proto_test_out.json";
	model.Store(fName, { true, true });
	std::cout << "Model written to the json file'" << fName << "':\n";
	std::cout << model.JSONStr({ true }) << "\n";
}

void CustomProtoTestLoad()
{
	std::cout << "Custom proto load test\n";
	Load("custom_proto_test_out.json");
}

void SimpleTest()
{
	std::cout << "Simple test begin\n";
	dmb::Model m;
	auto& registry = m.GetRegistry();
	auto& privateScope = m.GetPrivateScope();
	auto fruit = vl::Object();
	fruit.Set("title", "Fruit");
	auto orange = vl::Object();
	orange.Set("title", "Orange");
	orange.SetPrototype(fruit);
	auto& content = m.GetContent();
	content.GetData().Set("fruit", fruit);
	content.GetData().Set("orange", orange);
	const char* fName = "simple_test_out.json";
	m.Store(fName, { true, true });
	std::cout << m.JSONStr({ true }) << "\n";
}

void SimpleRemoveTest()
{
	std::cout << "Simple remove test begin\n";
	dmb::Model m;
	auto& registry = m.GetRegistry();
	auto& privateScope = m.GetPrivateScope();
	auto fruit = vl::Object();
	fruit.Set("title", "Fro");
	fruit.RemoveProperty("title");
	fruit.Set("title", "Fruit");
	auto orange = vl::Object();
	orange.Set("title", "Orange");
	orange.SetPrototype(fruit);
	auto& content = m.GetContent();
	content.GetData().Set("fruit", fruit);
	content.GetData().Set("orange", orange);
	const char* fName = "simple_remove_test.json";
	m.Store(fName, { true, true });
	std::cout << m.JSONStr({ true }) << "\n";
	Load(fName);
}

void SimpleRemoveArrayTest()
{
	std::cout << "Simple remove array test begin\n";
	dmb::Model m;
	auto& registry = m.GetRegistry();
	auto& privateScope = m.GetPrivateScope();
	auto fruit = vl::Object();
	fruit.Set("title", "Fruit");
	auto orange = vl::Object();
	orange.SetPrototype(fruit);
	orange.Set("title", "Orange");
	auto pear = vl::Object();
	pear.Set("title", "Pear");
	pear.SetPrototype(fruit);
	auto& content = m.GetContent();
	auto& fruitList = content.GetData().Set("fruitList", vl::List()).AsList();
	fruitList.Add(orange);
	fruitList.Remove(0);
	fruitList.Add(fruit);
	fruitList.Add(fruit);
	fruitList.Set(0, orange);
	fruitList.Add(pear, 0);
	const char* fName = "simple_test_remove_array_out.json";
	m.Store(fName, { true, true });
	std::cout << m.JSONStr({ true }) << "\n";
	Load(fName);
}

void RemoveTest()
{
	std::cout << "Remove test begin\n";
	dmb::Model m;
	auto& registry = m.GetRegistry();
	auto& privateScope = m.GetPrivateScope();
	auto fruit = vl::Object();
	fruit.Set("title", "Fro");
	fruit.RemoveProperty("title");
	fruit.Set("title", "Fruit");
	auto orange = vl::Object();
	orange.Set("title", "Orange");
	orange.SetPrototype(fruit);
	auto apple = vl::Object();
	apple.Set("title", "Apple");
	apple.SetPrototype(fruit);
	auto& content = m.GetContent();
	content.GetData().Set("fruit", fruit);
	content.GetData().Set("orange", orange);
	content.GetData().Set("apple", apple);
	auto& fruitList = content.GetData().Set("fruitList", vl::List()).AsList();
	fruitList.Add(fruit);
	fruitList.Add(orange);
	fruitList.Add(apple);
	content.Remove("fruit");
	const char* fName = "remove_test.json";
	m.Store(fName, { true, true });
	std::cout << m.JSONStr({ true }) << "\n";
	Load(fName);
}

void SimpleInTest()
{
	std::cout << "Simple in test\n";
	Load("simple_test_out.json");
}

void SimpleLoadTest()
{
	std::cout << "Simple load test\n";
	Load("simple_load_test.json");
}

void SimpleProtoWriteTest()
{
	std::cout << "Simple proto test\n";
	dmb::Model model;
	auto& content = model.GetContent();
	auto& registry = model.GetRegistry();
	auto& animals = content.Add("animalsAround", vl::List()).AsList();
	auto& privateScope = model.GetPrivateScope();
	auto& animal = registry.CreateType("animal");
	animal.Set("type", "");
	vl::Object bear;
	bear.SetPrototype(animal);
	bear.Set("type", "bear");
	animals.Add(bear);
	const char* fName = "simple_proto_test_out.json";
	model.Store(fName, { true });
	std::cout << "Written to '" << fName << "': \n" << model.JSONStr({ true }) << "\n";
}

void SimpleProtoLoadTest()
{
	std::cout << "Simple proto load test\n";
	Load("simple_proto_test_out.json");
}

void DatabaseTest()
{
	std::cout << "Database test\n";
	Load("database_test.json");
}

int main(int argc, const char* argv[])
{
	std::cout << "DataModelBuilderCore Test Project\n";
	RemoveTest();
	SimpleRemoveTest();
	SimpleRemoveArrayTest();
	//return 0;
	SimpleTest();
	SimpleInTest();
	//return 0;
	SimpleLoadTest();
	CrashTest();
	SimpleProtoWriteTest();
	SimpleProtoLoadTest();
	//return 0;
	CustomProtoTestStore();
	CustomProtoTestLoad();
	ModelCreationTest();
	ModelEditTest();
	ContentFillingTest();
	SeparateContentStoreTest();
	RenameTest();
	DatabaseTest();
	return 0;
}
