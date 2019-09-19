
#include "Scene_Generator.hpp"

#include "GraphIO.hpp"

Scene_Menu::Scene_Menu(Core core)
: Scene(core)
, gl(core.resources->Font("font"))
{}

Scene_Menu::~Scene_Menu() {}

void Scene_Menu::init()
{
    generated = false;

    model = Model::GRAPH;
    tmpModel = 0;
    numNodes = 0;
    density = 0.5f;
    distribution = Distribution::GRID;
    tmpDistribution = 0;
}

void Scene_Menu::handleEvents(const sf::Event& event)
{
    if(not ImGui::IsMouseHoveringAnyWindow() or event.type == sf::Event::MouseButtonReleased)
    {
        gl.handleEvents(event);
    }
}

void Scene_Menu::update(const sf::Time deltatime)
{
    ImGui::SetNextWindowSize(ImVec2(140, 190));
    ImGui::SetNextWindowPos(ImVec2(core.window->getSize().x - 145, 5));
    ImGui::Begin("MENU", NULL, ImGuiWindowFlags_NoResize);
    ImGui::RadioButton("graph", &tmpModel, 0);
    ImGui::SameLine(0, 8.f);
    ImGui::RadioButton("tree", &tmpModel, 1);
    ImGui::Separator();
    ImGui::PushItemWidth(45.f);
    ImGui::InputScalar(" layout", ImGuiDataType_U32, &numNodes);
    ImGui::Separator();
    if(Model(tmpModel) == Model::TREE)
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    ImGui::PushItemWidth(80.f);
    ImGui::SliderFloat(" dens.", &density, 0.f, 1.f);
    if(Model(tmpModel) == Model::TREE)
    {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    ImGui::Separator();
    ImGui::PushItemWidth(80.f);
    int size = reinterpret_cast<intptr_t>(gl.getDistributions()[0]);
    ImGui::Combo(" dist.", &tmpDistribution, &gl.getDistributions()[1], size);
    ImGui::Separator();
    if(ImGui::Button("GENERATE", ImVec2(125, 20)))
    {
        model = Model(tmpModel);
        distribution = Distribution(tmpDistribution);

        if(numNodes > 0)
        {
            graph = new Graph_AL();
            GraphGenerator::generate<Graph_AL>(*graph, model, Type::UNDIRECTED, numNodes, density);
            gl.load(*graph, distribution);

            generated = true;
        }
    }
    ImGui::Separator();
    if(ImGui::Button("SAVE", ImVec2(125, 20)))
    {
        if(generated) writeFile<Graph_AL>(*graph, "output.txt");
    }
    ImGui::End();

    gl.update(sf::Mouse::getPosition(*core.window));
}

void Scene_Menu::draw(sf::RenderWindow& window) const
{
    gl.draw(window);
}

void Scene_Menu::pause() {}

void Scene_Menu::resume() {}
