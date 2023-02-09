import json
import random

HORIZON = 60


def extract_param_from_list(param_name: str, param_list, param_key: str):
    """Convert a list parameter to a .dat format"""
    string_data = "\n" + "param " + param_name + ":=\n"
    for i in range(len(param_list)):
        string_data += str(i + 1) + " " + str(param_list[i][param_key]) + "\n"
    string_data += ";\n"
    return string_data


def extract_param(param_name: str, param):
    """Convert a simple parameter to a .dat format"""
    return "param " + param_name + ":=" + str(param) + ";\n"


def extract_apport_eau(horizon, scenarios):
    """Convert the apport_hebdomadaire parameter to a .dat format"""
    string_data = "\n" + "param " + "Apport_Eau : "
    for i in range(horizon):
        string_data += str(i) + " "
    string_data += ":=\n"
    for s in range(len(scenarios)):
        string_data += str(s + 1) + " "
        data_list = scenarios[s]["apport_hebdomadaire"]
        for t in range(len(data_list)):
            string_data += str(data_list[t]) + " "
        string_data += "\n"
    string_data += ";\n"
    return string_data


class Culture:
    def __init__(self, c_id, name="c", water=random.randrange(1, 13) / 10, duration=random.randrange(20, 30),
                 land=random.randrange(100, 200) / 1000, reward=random.randrange(50, 150),
                 es=random.randrange(0, 15), ls=random.randrange(15, 30), emission=random.randrange(1, 5)):
        self.id = c_id
        self.nom = name
        self.besoin_eau = water
        self.duree_pousse = duration
        self.hectars_pour_tonne = land
        self.rendement = reward
        self.depart_tot = es
        self.depart_tard = ls
        self.emission = emission


class Scenario:
    def __init__(self, horizon, initial=random.randrange(50, 100), range_hebdo=(0, 25)):
        self.apport_initial = initial
        self.apport_hebdomadaire = [random.randrange(range_hebdo[0], range_hebdo[1])
                                    for k in range(horizon)]  # En mm par semaine


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
        data += extract_apport_eau(self.horizon, self.scenarios)
        # print(data)
        with open(file_name, "w") as file:
            file.write(data)


class Cereal(Instance):

    def __init__(self, job, scenario):
        super().__init__(job, scenario)
        cereal_name = ["ble", "mais", "tournesol", "colza", "orge", "sorgho", "avoine", "sarrasin", "seigle", "millet"]
        self.horizon = HORIZON
        self.nb_hectars = 80
        self.GESMAX = 800
        self.cultures = [self.create_crop(k, cereal_name[k]) for k in range(self.nb_jobs)]
        self.scenarios = [Scenario(self.horizon, 50, (0, 15)).__dict__ for k in range(self.nb_scenarios)]

    def create_crop(self, c_id, name):
        water = random.randrange(1, 13) / 10
        duration = random.randrange(20, 30)
        land = random.randrange(100, 200) / 1000
        reward = random.randrange(50, 150)
        early = random.randrange(0, 15)
        latest = random.randrange(early, self.horizon - duration)
        emission = random.randrange(1, 5)
        result = Culture(c_id, name, water, duration, land, reward, early, latest, emission).__dict__
        return result


if __name__ == "__main__":
    default_path = "cmake-build-debug\\Stat\\generated_test_instance"
    #jsons_path = "cmake-build-debug\\Instances\\generatedInstanceNew"
    #dat_path = "..\\model\\latest_model"
    n_job = (5,8,10)
    n_scenar = 3
    for j in range(90):
        inst = Cereal(n_job[j//30], n_scenar)
        inst.to_json(default_path + str(j) + ".json")
        inst.to_dat(default_path + str(j) + ".dat")
