import json
import random


class Culture:
    def __init__(self):
        self.nom = random.choice(["ble", "mais", "tournesol", "colza"])
        self.besoin_eau = random.randrange(1, 70)/10
        self.duree_pousse = random.randrange(10, 28)
        self.hectars_pour_tonne = 0.2
        self.rendement = 80
        self.depart_tot = 2
        self.depart_tard = 4
        self.emission = 60


class Scenario:
    def __init__(self, horizon):
        self.apport_initial = random.randrange(50, 100)  # Pluviométrie du mois précédent
        self.apport_hebdomadaire = [random.randrange(4, 35) for k in range(horizon)]  # En mm par semaine


class Instance:
    def __init__(self):
        self.id = 1
        self.nb_jobs = 2
        self.horizon = 30
        self.nb_scenarios = 3
        self.nb_hectars = 80
        self.GESMAX = 800
        self.cultures = [Culture().__dict__ for k in range(self.nb_jobs)]
        self.scenarios = [Scenario(self.horizon).__dict__ for k in range(self.nb_scenarios)]


if __name__ == "__main__":
    filename = "cmake-build-debug\Instances\generatedInstance2.json"
    data = Instance().__dict__
    print(data["cultures"])
    with open(filename, "w") as f:
        json.dump(data, f)
