/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Michálek Kryštof (xmicha94)
Description: Komponenta zóny pro přetahování tlačítek, která zpracovává registraci uživatelů.
*/

import React from 'react';
import { useDrop } from 'react-dnd';

// Rozhraní pro vlastnosti komponenty DropZone
interface DropZoneProps {
  acceptRole: 'visitor' | 'worker'; // Role, kterou zóna přijímá
  isRegistered: boolean; // Stav registrace uživatele
  handleDrop: (role: 'visitor' | 'worker', action: 'register' | 'unregister') => void; // Funkce volaná při drop události
}

const DropZone: React.FC<DropZoneProps> = ({ acceptRole, isRegistered, handleDrop }) => {
  // Nastavení funkcionality drop zóny
  const [{ isOver, canDrop }, drop] = useDrop({
    accept: 'BUTTON', // Typ objektu, který zóna přijímá
    drop: (item: { action: 'register' | 'unregister' }) => handleDrop(acceptRole, item.action), // Akce při drop události
    collect: monitor => ({
      isOver: !!monitor.isOver(), // Stav, zda je objekt nad zónou
      canDrop: !!monitor.canDrop(), // Stav, zda je možné objekt pustit
    }),
  });

  const isActive = isOver && canDrop; // Aktivní stav zóny

  return (
    <div
      ref={drop} // Připojení funkce drop k zóně
      className={`p-4 border rounded-md ${
        isRegistered ? 'bg-green-300' : isActive ? 'bg-green-200' : 'bg-gray-200'
      }`}
      style={{ minHeight: '50px', textAlign: 'center', marginBottom: '10px' }}
    >
      {isRegistered
        ? acceptRole === 'visitor'
          ? 'Přihlášen na akci jako návštěvník'
          : 'Přihlášen na akci jako dobrovolník'
        : acceptRole === 'visitor'
        ? 'Přihlásit na akci jako návštěvník?'
        : 'Přihlásit na akci jako dobrovolník?'}
    </div>
  );
};

export default DropZone;
