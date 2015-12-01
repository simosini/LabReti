class RisorsaCondivisa {
	private int counter;
	
	public RisorsaCondivisa() {
		this.counter = 0;
		System.out.println("Inizializzato il contatore condiviso.");
	}

	public synchronized int incrementa(int valore)
	{
		counter += valore;
		return counter;
	}
}
