class Comment < ActiveRecord::Base
	def validate_comment
		if (comment == "") then
			errors.add(:comment, "Comment cannot be empty.")
		end
	end
	validate :validate_comment
	belongs_to :photo
	belongs_to :user
end
