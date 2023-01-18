import json
import random

HORIZON = 30


class Culture:
    def __init__(self, id):
        self.id = id
        self.nom = random.choice(["ble", "mais", "tournesol", "colza"])
        self.besoin_eau = random.randrange(1, 70) / 10
        self.duree_pousse = random.randrange(2, 28)
        self.hectars_pour_tonne = 0.2
        self.rendement = 80
        self.depart_tot = 2
        self.depart_tard = random.randrange(2, HORIZON - self.duree_pousse)
        self.emission = 60


class Scenario:
    def __init__(self, horizon):
        self.apport_initial = random.randrange(50, 100)  # Pluviométrie du mois précédent
        self.apport_hebdomadaire = [random.randrange(4, 35) for k in range(horizon)]  # En mm par semaine


def extract_param_from_list(param_name: str, param_list, param_key: str):
    string_data = "\n" + "param " + param_name + ":=\n"
    for i in range(len(param_list)):
        string_data += str(i+1) + " " + str(param_list[i][param_key]) + "\n"
    string_data += ";\n"
    return string_data


def extract_param(param_name: str, param):
    return "param " + param_name + ":=" + str(param) + ";\n"


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

    def extract_apport_eau(self):
        string_data = "\n"+"param "+"Apport_Eau : "
        for i in range(self.horizon):
            string_data += str(i+1) + " "
        string_data += ":=\n"
        for s in range(len(self.scenarios)):
            string_data += str(s+1) + " "
            data_list = self.scenarios[s]["apport_hebdomadaire"]
            for t in range(len(data_list)):
                string_data += str(data_list[t]) + " "
            string_data += "\n"
        string_data += ";\n"
        return string_data

    def to_json(self, file_name):
        with open(file_name, "w") as file:
            json.dump(self.__dict__, file)

    def to_dat(self, file_name):
        data = ""
        data += extract_param("HV", 1000)
        data += extract_param("T", self.horizon)
        data += extract_param("Q", self.nb_hectars)
        data += extract_param("GESMAX", self.GESMAX)
        data += extract_param("n", self.nb_jobs)

        data += extract_param_from_list("Besoin_Eau", self.cultures, "besoin_eau")
        data += extract_param_from_list("p", self.cultures, "duree_pousse")
        data += extract_param_from_list("a", self.cultures, "hectars_pour_tonne")
        data += extract_param_from_list("r", self.cultures, "rendement")
        data += extract_param_from_list("es", self.cultures, "depart_tot")
        data += extract_param_from_list("ls", self.cultures, "depart_tard")
        data += extract_param_from_list("EC", self.cultures, "emission")

        data += extract_param("S", self.nb_scenarios)
        data += extract_param_from_list("Volume_Eau_Initial", self.scenarios, "apport_initial")
        data += self.extract_apport_eau()
        # print(data)
        with open(file_name, "w") as file:
            file.write(data)


if __name__ == "__main__":
    jsons_path = "cmake-build-debug\\Instances\\generatedInstance"
    dat_path = "..\\model\\latest_model"

    for j in range(6):
        job = 5
        scenar = 3
        inst = Instance(job, scenar)
        inst.to_json(jsons_path+str(j)+".json")
        inst.to_dat(dat_path+str(j)+".dat")
        job += 5 * (j % 2)
        scenar += 3 * ((j + 1) % 2)
