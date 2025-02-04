/*
Author: Michálek Kryštof (xmicha94)
Description: Komponenta tlačítka, které lze přetahovat pomocí knihovny react-dnd.
*/

import React from 'react';
import { useDrag } from 'react-dnd';

// Rozhraní pro vlastnosti komponenty DraggableButton
interface DraggableButtonProps {
  text: string; // Text zobrazený na tlačítku
  role: 'visitor' | 'worker'; // Role uživatele
  action: 'register' | 'unregister'; // Akce provedená při přetažení
  onDrop: (role: 'visitor' | 'worker', action: 'register' | 'unregister') => void; // Funkce spuštěná při drop události
  color: 'green' | 'red'; // Barva tlačítka
}

const DraggableButton: React.FC<DraggableButtonProps> = ({ text, role, action, onDrop, color }) => {
  // Nastavení funkcionality drag-and-drop pomocí useDrag
  const [{ isDragging }, drag] = useDrag({
    type: 'BUTTON', // Typ objektu pro drag-and-drop
    item: { action }, // Data předaná při přetažení
    collect: monitor => ({
      isDragging: !!monitor.isDragging(), // Stav, zda je tlačítko právě přetahováno
    }),
  });

  return (
    <button
      ref={drag} // Připojení funkce drag k tlačítku
      className={`${color === 'green' ? 'bg-green-500' : 'bg-red-500'} text-white px-6 py-2 rounded-md mx-2 hover:${color === 'green' ? 'bg-green-600' : 'bg-red-600'}`}
      style={{ opacity: isDragging ? 0.5 : 1 }} // Snížení průhlednosti při přetahování
    >
      {text}
    </button>
  );
};

export default DraggableButton;
