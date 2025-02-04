/*
ITU Projekt
Webová aplikace - Akce pro lidi
Author: Marek Konečný (xkonec85)
Description: Komponenta pro zónu pro přetažení kategorií
*/
import React from 'react';
import { useDrop } from 'react-dnd';

// Define the props for the CategoryDropZone component
interface CategoryDropZoneProps {
  onDrop: (category: string) => void; // Function to handle the drop event
}

// CategoryDropZone component
const CategoryDropZone: React.FC<CategoryDropZoneProps> = ({ onDrop }) => {
  // useDrop hook to handle the drop functionality
  const [{ isOver, canDrop }, drop] = useDrop({
    accept: 'CATEGORY', // Accepts items of type 'CATEGORY'
    drop: (item: { category: string }) => onDrop(item.category), // Calls onDrop with the dropped category
    collect: (monitor) => ({
      isOver: !!monitor.isOver(), // Monitor if an item is being dragged over the drop zone
      canDrop: !!monitor.canDrop(), // Monitor if the item can be dropped
    }),
  });

  // Determine if the drop zone is active
  const isActive = isOver && canDrop;

  return (
    // Render the drop zone
    <div
      ref={drop} // Attach the drop ref to the div
      className={`p-4 border rounded-md ${isActive ? 'bg-green-200' : 'bg-gray-200'}`} // Apply styles based on the active state
      style={{ minHeight: '50px', textAlign: 'center', marginBottom: '10px' }} // Inline styles
    >
      {isActive ? 'Release to drop' : 'Drag a category here'}
    </div>
  );
};

export default CategoryDropZone; // Export the component