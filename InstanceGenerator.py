import json
import random

HORIZON = 30
class Culture:
    def __init__(self, id):
        self.id = id;
        self.nom = random.choice(["ble", "mais", "tournesol", "colza"])
        self.besoin_eau = random.randrange(1, 70)/10
        self.duree_pousse = random.randrange(2, 28)
        self.hectars_pour_tonne = 0.2
        self.rendement = 80
        self.depart_tot = 2
        self.depart_tard = random.randrange(2, HORIZON-self.duree_pousse)
        self.emission = 60


class Scenario:
    def __init__(self, horizon):
        self.apport_initial = random.randrange(50, 100)  # Pluviométrie du mois précédent
        self.apport_hebdomadaire = [random.randrange(4, 35) for k in range(horizon)]  # En mm par semaine


class Instance:
    def __init__(self, job, scenario):
        self.id = 1
        self.nb_jobs = job
        self.horizon = HORIZON
        self.nb_scenarios = scenario
        self.nb_hectars = 80
        self.GESMAX = 800
        self.cultures = [Culture(k).__dict__ for k in range(self.nb_jobs)]
        self.scenarios = [Scenario(self.horizon).__dict__ for k in range(self.nb_scenarios)]


if __name__ == "__main__":
    path = "cmake-build-debug\Instances\generatedInstance"
    extension = ".json"
    for j in range(6):
        job = 5
        scenar = 3
        filename = path+str(j)+extension
        data = Instance(job, scenar).__dict__
        # print(data["cultures"])
        with open(filename, "w") as f:
            json.dump(data, f)
        job+=5*(j%2)
        scenar += 3*((j+1)%2)