
struct enfermero{

int tipo; //0, 1 o 2: 0 Junior, 1 Medios, 2 Senior
pthread_t hiloEnfermero;
};
void *accionesEnfermero(void *arg){

	int tipo = *(int *)arg;
	char cadena[50]="";
	int deNingunTipo = 0;
	int posicionDelIDPrioritario = 0;
	int contadorJunior=0;
	int contadorMedios=0;
	int contadorSenior=0;

	sprintf(cadena,"Enfermero_%d",tipo);
	

	if(tipo == 1){
		while(1){
		//Enfermero Junior
		pthread_mutex_lock(&semaforoPacientes);
		
			posicionDelIDPrioritario = buscarPacienteInvitacion();
			pthread_mutex_unlock(&semaforoPacientes);
		
			if(posicionDelIDPrioritario == -9){
				//no hay de su tipo, entonces busca del otro tipo
				pthread_mutex_lock(&semaforoPacientes);
				deNingunTipo = buscarPacienteSenior();

				if(deNingunTipo==0){
					deNingunTipo=buscarPacienteMedio();
				}

				pthread_mutex_unlock(&semaforoPacientes);

					if(deNingunTipo == -9){
						
						sleep(1);

					}else{					
						contadorMedios++;
						atenderPaciente(deNingunTipo,tipo);
						
					}
			}else{ //lo ha encontrado en la lista, calcula tiempo de atencion
				//pthread_mutex_unlock(&semaforoPacientes);
				contadorMedios++;
				atenderPaciente(posicionDelIDPrioritario,tipo);
				
			}

			if(contadorMedios==5){
				
					pthread_mutex_lock(&semaforoFichero);
					writeLogMessage(cadena,"Ya he atendido a 5 pacientes, me toca tomar el café");
					pthread_mutex_unlock(&semaforoFichero);

							sleep(5);
							contadorMedios=0;

					pthread_mutex_lock(&semaforoFichero);
					writeLogMessage(cadena,"Se acabo mi descanso, vuelvo al trabajo");
					pthread_mutex_unlock(&semaforoFichero);
			}
		
		}

	}


	if(tipo == 2){
		//Enfermero Medios
		while(1){
		pthread_mutex_lock(&semaforoPacientes);

			posicionDelIDPrioritario = buscarPacienteJunior();
			pthread_mutex_unlock(&semaforoPacientes);

			if(posicionDelIDPrioritario == -9){
				//no hay de su tipo, entonces busca del otro tipo 
				pthread_mutex_lock(&semaforoPacientes);

				deNingunTipo = buscarPacienteSenior();

				if(deNingunTipo==0){
					deNingunTipo=buscarPacienteJunior();
				}

				pthread_mutex_unlock(&semaforoPacientes);

					if(deNingunTipo == -9){
						
						sleep(1);

					}else{
						contadorMedios++;
						atenderPaciente(deNingunTipo,tipo);
					}
			}else{ //lo ha encontrado en la lista, calcula tiempo de atencion
				//pthread_mutex_unlock(&semaforoPacientes);
				contadorMedios++;
				atenderPaciente(posicionDelIDPrioritario,tipo);
			}

			if(contadorMedios==5){

					pthread_mutex_lock(&semaforoFichero);
					writeLogMessage(cadena,"Ya he atendido a 5 pacientes, me toca tomar el café");
					pthread_mutex_unlock(&semaforoFichero);

				sleep(5);
				contadorMedios=0;

					pthread_mutex_lock(&semaforoFichero);
					writeLogMessage(cadena,"Se acabo mi descanso, vuelvo al trabajo");
					pthread_mutex_unlock(&semaforoFichero);
			}
		

	}
	}

	

		
		if(tipo == 3){
			//Enfermero Senior
		while(1){
			pthread_mutex_lock(&semaforoPacientes);

			posicionDelIDPrioritario = buscarPacienteSenior();


			pthread_mutex_unlock(&semaforoPacientes);

			if(posicionDelIDPrioritario == -9){
							
				sleep(1);


			}else{ //lo ha encontrado en la lista, calcula tiempo de atencion
				contadorSenior++;
				atenderPaciente(posicionDelIDPrioritario,tipo);
			}

			if(contadorSenior==5){

					pthread_mutex_lock(&semaforoFichero);
					writeLogMessage(cadena,"Ya he atendido a 5 pacientes, me toca tomar el café");
					pthread_mutex_unlock(&semaforoFichero);

				sleep(5);
				contadorSenior=0;

					pthread_mutex_lock(&semaforoFichero);
					writeLogMessage(cadena,"Se acabo mi descanso, vuelvo al trabajo");
					pthread_mutex_unlock(&semaforoFichero);
			}
						
		}

	}
	
}