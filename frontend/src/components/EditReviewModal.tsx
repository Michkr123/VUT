import React, { useState, useEffect } from 'react';
import Rating from '@mui/material/Rating';

const EditReviewModal = ({ review, isOpen, onClose, onSave }) => {
  const [updatedReview, setUpdatedReview] = useState({
    id: 0,
    username: '',
    comment: '',
    rating: 2.5,
    date_posted: ''
  });

  useEffect(() => {
    if (review) {
      setUpdatedReview(review);
    }
  }, [review]);

  if (!isOpen) return null;

  const handleSave = () => {
    onSave(updatedReview);
    onClose();
  };

  return (
    <div className="fixed inset-0 bg-gray-600 bg-opacity-50 flex items-center justify-center">
      <div className="bg-white p-6 rounded-lg shadow-lg w-full max-w-md">
        <h2 className="text-2xl font-bold mb-4">Edit Review</h2>
        <form>
          <div className="mb-4">
            <label htmlFor="rating" className="block text-sm font-medium text-gray-700">Rating</label>
            <Rating
              name="half-rating"
              value={updatedReview.rating}
              precision={0.5}
              onChange={(event, newValue) => setUpdatedReview({ ...updatedReview, rating: newValue })}
            />
          </div>
          <div className="mb-4">
            <label htmlFor="comment" className="block text-sm font-medium text-gray-700">Comment</label>
            <textarea
              id="comment"
              value={updatedReview.comment}
              onChange={(e) => setUpdatedReview({ ...updatedReview, comment: e.target.value })}
              rows={4}
              className="mt-1 block w-full rounded-md border-gray-300 shadow-sm focus:border-indigo-500 focus:ring-indigo-500 sm:text-sm"
              required
            />
          </div>
          <div className="flex justify-end space-x-2">
            <button type="button" className="bg-gray-300 text-black py-2 px-4 rounded-lg hover:bg-gray-400" onClick={onClose}>Cancel</button>
            <button type="button" className="bg-indigo-600 text-white py-2 px-4 rounded-lg hover:bg-indigo-700" onClick={handleSave}>Save</button>
          </div>
        </form>
      </div>
    </div>
  );
};

export default EditReviewModal;
