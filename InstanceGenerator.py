import json
class Culture:
    def __init__(self):
        self.nom = "ble"
        self.besoin_eau = 0.5
        self.duree_pousse = 20
        self.hectars_pour_tonne = 0.2
        self.rendement = 80
        self.depart_tot = 2
        self.depart_tard = 4
        self.emission = 60
class Scenario:
    def __init__(self, horizon):
        self.apport_initial = 800
        self.apport_hebdomadaire = [10 for k in range(horizon)]
class Instance:
    def __init__(self):
        self.id = 1
        self.nb_job = 1
        self.horizon = 30
        self.nb_scenario = 1
        self.nb_hectars = 80
        self.GESMAX = 800
        self.cultures = [Culture().__dict__ for k in range(self.nb_job)]
        self.scenarios = [Scenario(self.horizon).__dict__ for k in range(self.nb_scenario)]


if __name__ == "__main__":
    filename = "cmake-build-debug\Instances\generatedInstance1.json"
    data = Instance().__dict__
    with open(filename, "w") as f:
        json.dump(data, f)

