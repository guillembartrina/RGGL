
#include "Scene_Menu.hpp"

Scene_Menu::Scene_Menu(Core core)
: Scene(core)
, gl(core.resources->Font("font"))
{}

Scene_Menu::~Scene_Menu() {}

void Scene_Menu::init()
{
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
    ImGui::SetNextWindowSize(ImVec2(140, 180));
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
    //std::cerr << &gl.getDistributions()[0] << std::endl;
    //std::cerr << gl.getDistributions()[1] << std::endl;
    //std::cerr << &gl.getDistributions()[1] << std::endl;
    //std::cerr << (gl.getDistributions()+sizeof(char*)) << std::endl;
    ImGui::Combo(" dist.", &tmpDistribution, &gl.getDistributions()[1], size);
    ImGui::Separator();
    if(ImGui::Button("GEN", ImVec2(125, 20)))
    {
        model = Model(tmpModel);
        distribution = Distribution(tmpDistribution);

        if(numNodes > 0)
        {
            Graph_AL graph;
            GraphGenerator::generate(graph, model, Type::UNDIRECTED, numNodes, density);
            gl.load(graph, distribution);
        }
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
