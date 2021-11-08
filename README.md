# Mysterio 👾
#### Guia de planejamento do Mysterio Framework

Para construir uma aplicação usando este framework, indicamos a nossa arquitetura que pode ser vista na figura a seguir:

![Mysterio Framework e seus componentes](UAV architecture-Componente.png)

<p style="text-align: center;">Arquitetura de Software do Mysterio Framework</p>

## Recomendações para construção de sistemas Multi-UAVs utilizando esta arquitetura e o Mysterio Framework:

#### UAVs e UAVCommunication
Para os UAVs, sejam virtuais ou reais, o software de gerenciamento interno dos UAVs tem que ser a sua maneira (desenvolvida pelo usuário), de forma que use o UAVCommunication como interface de comunicação entre o framework e os UAVs. É importante deixar claro que o framework deverá estar presente na sua estação base (central de comandos). Já nos UAVs, a comunicação com o framework se dá pelo UAVCommunication que é a interface de comunicação. Esta possui por padrão métodos que permite a conexão e a desconexão com o framework, além de envio e recebimento de mensagens.

#### Communication
Este é o componente responsável por realizar toda e qualquer comunicação do framework com os UAVs. O UAVCommunication e este componente devem utilizar-se do mesmo protocolo de comunicação para se comunicarem. Conforme este componente recebe mensagens, ele repassa para os outros componentes que lidam com cada tipo específico de informação.

#### StatusManager

Componente responsável pelo status (propriedades, atributos e características) dos UAVs.

O ideal é implementar, neste componente do framework, métodos referentes a cada propriedade de Status dos UAVs. Dependendo da aplicação, um Sistema Multi-UAV possui muitos dados que devem ser repassados do UAV pro Framework. Esses são alguns dos Status que podem ser relevantes:

> Observação: É importante compreender que quando lidamos com UAVs Heterogêneos pode possuir algum UAV que não suporte todas as propriedades/características listadas, além de poder incluir novas.

Dados de Status dos UAVs:
* Bateria
* Tempo de vôo
* Altitude e Localização/posição geográfica
* Velocidade
* Carga útil
* Disponibilidade (comunicação ou afazeres)
* Ociosidade

> Usar uma classe status, onde cada informação considerada como Status deve virar um atributo da classe ou derivar novas classes que armazenem diferentes atributos cada.

Na Status Manager deve-se fazer pelo menos um método para cada Status, onde o componente repassará o status do(s) UAV(s) para outros componentes.

#### Repository
Componente de persistência de dados, seja de Status dos UAVs ou outras informações, esse que é responsável por armazenar no banco de dados, mas o ideal é saber lidar com toda a informação que recebe, se é ou não relevante para ser armazenada.

#### Task
Classe que representa as tarefas, estas que podem ser abordadas de 2 maneiras previstas. A primeira seria não especificar diretamente como realizar a tarefa, mas apena indicar por meio dos atributos como diferenciar aquela tarefa e o que ela representa, que o UAV já deve interpretar e realizar ações pré-programadas. Já a segunda maneira de trabalhar com tarefas seria passar para a tarefa os comandos que o UAV deve executar para realizar aquela tarefa.

#### TaskManager
Componente responsável por gerenciar e receber informações das tarefas.

#### MissionPlanner
Componente que tem toda a autonomia de gerenciamento da missão, esta que é composta por um conjunto de tarefas designadas aos UAVs.