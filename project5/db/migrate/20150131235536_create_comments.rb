class CreateComments < ActiveRecord::Migration
  def change
    create_table :comments do |t|
    	t.integer :photo_id
    	t.integer :user_id
    	t.datetime :date_time
    	t.string :comment
    end
  end
end
