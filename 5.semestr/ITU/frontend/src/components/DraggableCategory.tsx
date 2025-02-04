/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Marek Konečný (xkonec85)
Description: Komponenta pro přetažení kategorií
*/
import React from 'react';
import { useDrag } from 'react-dnd';

// Define the props for the DraggableCategory component
interface DraggableCategoryProps {
  category: string; // The category to be dragged
}

// DraggableCategory component
const DraggableCategory: React.FC<DraggableCategoryProps> = ({ category }) => {
  // useDrag hook to handle the drag functionality
  const [{ isDragging }, drag] = useDrag({
    type: 'CATEGORY', // Type of the draggable item
    item: { category }, // The item being dragged
    collect: (monitor) => ({
      isDragging: !!monitor.isDragging(), // Monitor if the item is being dragged
    }),
  });

  return (
    // Render the draggable category
    <div
      ref={drag} // Attach the drag ref to the div
      className="bg-blue-500 text-white px-4 py-2 rounded-md mx-2 cursor-pointer" // Apply styles
      style={{ opacity: isDragging ? 0.5 : 1 }} // Change opacity based on the dragging state
    >
      {category}
    </div>
  );
};

export default DraggableCategory; // Export the component